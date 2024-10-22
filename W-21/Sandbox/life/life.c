#include "universe.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void evolve(Universe *a, Universe *b, bool toroidal) {
    for (int m = 0; m < uv_rows(a); m += 1) {
        for (int n = 0; n < uv_cols(a); n += 1) {
            int alive = uv_get_cell(a, m, n);
            int count = uv_census(a, m, n, toroidal);
            if (alive && (count == 2 || count == 3)) {
                uv_set_cell(b, m, n); // Survival.
            } else if (!alive && count == 3) {
                uv_set_cell(b, m, n); // New birth.
            } else {
                uv_clr_cell(b, m, n); // Overcrowding or loneliness.
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

#define DELAY   50000
#define OPTIONS "tsn:i:o:"

int main(int argc, char **argv) {
    int opt = 0;
    int iterations = 10;
    bool silent = false;
    bool toroidal = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 't':
                toroidal = true;
                break;
            case 's':
                silent = true;
                break;
            case 'n':
                iterations = atoi(optarg);
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

    int m = 0, n = 0;
    if (fscanf(infile, "%d %d\n", &m, &n) != 2) {
        fprintf(stderr, "Malformed input.\n");
        return 1;
    }

    Universe *a = uv_create(m, n);
    Universe *b = uv_create(m, n);
    uv_populate(a, infile);

    initscr();
    noecho();
    curs_set(FALSE);

    for (int i = 0; i < iterations; i += 1) {
        if (!silent) {
            clear();
            show_grid(a);
            refresh();
            usleep(DELAY);
        }
        evolve(a, b, toroidal);
        Universe *c = a;
        a = b;
        b = c;
    }

    endwin();
    uv_print(a, outfile);
    return 0;
}
