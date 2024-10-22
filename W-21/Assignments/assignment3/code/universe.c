#include "universe.h"
#include <stdlib.h>

struct Universe {
    int rows;
    int cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(int rows, int cols, bool toroidal) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (int r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    return u;
}

void uv_delete(Universe *u) {
    if (u && u->grid) {
        for (int r = 0; r < u->rows; r += 1) {
            free(u->grid[r]);
        }
        free(u->grid);
        free(u);
    }
}

static inline bool valid(Universe *u, int r, int c) {
    return r >= 0 && r < u->rows && c >= 0 && c < u->cols;
}

int uv_rows(Universe *u) {
    return u->rows;
}

int uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, int r , int c) {
    if (valid(u, r, c)) {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, int r , int c) {
    if (valid(u, r, c)) {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, int r, int c) {
    return valid(u, r, c) ? u->grid[r][c] : false;
}

bool uv_populate(Universe *u, FILE *infile) {
    int r = 0, c = 0, status = 0;
    while ((status = fscanf(infile, "%d %d\n", &r, &c)) != EOF) {
        if (status != 2 || !valid(u, r, c)) {
            fprintf(stderr, "Malformed input.\n");
            return false;
        }
        u->grid[r][c] = true;
    }
    return true;
}

int uv_census(Universe *u, int r, int c) {
    int count = 0;
    for (int y = -1; y <= 1; y += 1) {
        for (int x = -1; x <= 1; x += 1) {
            if (y || x) {
                int r_ = u->toroidal ? (r + u->rows + y) % u->rows : r + y;
                int c_ = u->toroidal ? (c + u->cols + x) % u->cols : c + x;
                if (valid(u, r_, c_) && u->grid[r_][c_]) {
                    count += 1;
                }
            }
        }
    }
    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    for (int r = 0; r < u->rows; r += 1) {
        for (int c = 0; c < u->cols; c += 1) {
            fputc(u->grid[r][c] ? 'o' : '.', outfile);
        }
        fputc('\n', outfile);
    }
}
