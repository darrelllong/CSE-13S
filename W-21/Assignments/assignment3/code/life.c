#include "universe.h"
#include <errno.h>
#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void evolve(Universe *a, Universe *b) {
    for (int m = 0; m < uv_rows(a); m += 1) {
        for (int n = 0; n < uv_cols(a); n += 1) {
            bool alive = uv_get_cell(a, m, n);
            int count = uv_census(a, m, n);
            if (alive && (count == 2 || count == 3)) {
                uv_live_cell(b, m, n); // Survival.
            } else if (!alive && count == 3) {
                uv_live_cell(b, m, n); // New birth.
            } else {
                uv_dead_cell(b, m, n); // Overcrowding or loneliness.
            }
        }
    }
}

void show_grid(Universe *u) {
    for (int m = 0; m < uv_rows(u); m += 1) {
        for (int n = 0; n < uv_cols(u); n += 1) {
            if (uv_get_cell(u, m, n)) {
                mvprintw(m, n, "o");
            }
        }
    }
}

#define DELAY 50000
#define GENERATIONS 100
#define OPTIONS "tsn:i:o:"

int main(int argc, char **argv) {
    int opt = 0;
    bool silent = false;
    bool toroidal = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int generations = GENERATIONS;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            toroidal = true;
            break;
        case 's':
            silent = true;
            break;
        case 'n':
            generations = strtol(optarg, NULL, 10);
            if (errno == EINVAL) {
                fprintf(stderr, "Invalid number of generations.\n");
                return 1;
            }
            break;
        case 'i':
            if ((infile = fopen(optarg, "r")) == NULL) {
                fprintf(stderr, "Error opening %s.\n", optarg);
                return 1;
            }
            break;
        case 'o':
            if ((outfile = fopen(optarg, "w")) == NULL) {
                fprintf(stderr, "Error opening %s.\n", optarg);
                return 1;
            }
            fchmod(fileno(outfile), 0600);
            break;
        default:
            return 1;
        }
    }

    int r = 0, c = 0;
    if (fscanf(infile, "%d %d\n", &r, &c) != 2 || r < 0 || c < 0) {
        fprintf(stderr, "Malformed input.\n");
        return 1;
    }

    Universe *a = uv_create(r, c, toroidal);
    Universe *b = uv_create(r, c, toroidal);
    if (!uv_populate(a, infile)) {
        uv_delete(a);
        uv_delete(b);
        return 1;
    }

    if (!silent) {
        initscr();
        noecho();
        curs_set(FALSE);
    }

    for (int i = 0; i < generations; i += 1) {
        if (!silent) {
            clear();
            show_grid(a);
            refresh();
            usleep(DELAY);
        }
        evolve(a, b);
        Universe *c = a;
        a = b;
        b = c;
    }

    if (!silent) {
        endwin();
    }

    uv_print(a, outfile);
    uv_delete(a);
    uv_delete(b);
    fclose(infile);
    fclose(outfile);
    return 0;
}
