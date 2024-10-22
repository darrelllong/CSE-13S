#include "universe.h"
#include <inttypes.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    return u;
}

void uv_delete(Universe *u) {
    if (u && u->grid) {
        for (uint32_t r = 0; r < u->rows; r += 1) {
            free(u->grid[r]);
        }
        free(u->grid);
        free(u);
    }
}

static inline bool valid(Universe *u, uint32_t r, uint32_t c) {
    return r < u->rows && c < u->cols;
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r , uint32_t c) {
    if (valid(u, r, c)) {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r , uint32_t c) {
    if (valid(u, r, c)) {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    return valid(u, r, c) ? u->grid[r][c] : false;
}

bool uv_populate(Universe *u, FILE *infile) {
    int status = 0;
    uint32_t r = 0, c = 0;
    while ((status = fscanf(infile, "%" SCNu32 " %" SCNu32 "\n", &r, &c)) != EOF) {
        if (status != 2 || !valid(u, r, c)) {
            fprintf(stderr, "Malformed input.\n");
            return false;
        }
        u->grid[r][c] = true;
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t count = 0;
    for (int y = -1; y <= 1; y += 1) {
        for (int x = -1; x <= 1; x += 1) {
            if (y || x) {
                uint32_t r_ = u->toroidal ? (r + u->rows + y) % u->rows : r + y;
                uint32_t c_ = u->toroidal ? (c + u->cols + x) % u->cols : c + x;
                if (valid(u, r_, c_) && u->grid[r_][c_]) {
                    count += 1;
                }
            }
        }
    }
    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t r = 0; r < u->rows; r += 1) {
        for (uint32_t c = 0; c < u->cols; c += 1) {
            fputc(u->grid[r][c] ? 'o' : '.', outfile);
        }
        fputc('\n', outfile);
    }
}
