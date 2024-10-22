#include "universe.h"
#include <stdlib.h>

struct Universe {
    int m;
    int n;
    int **grid;
};

Universe *uv_create(int m, int n) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    u->grid = (int **) malloc(m * sizeof(int *));
    for (int i = 0; i < m; i += 1) {
        u->grid[i] = (int *) calloc(n, sizeof(int));
    }
    u->m = m;
    u->n = n;
    return u;
}

void uv_delete(Universe **u) {
    for (int i = 0; i < (*u)->m; i += 1) {
        free((*u)->grid[i]);
        (*u)->grid[i] = NULL;
    }
    free((*u)->grid);
    (*u)->grid = NULL;
    free(*u);
    *u = NULL;
}

static inline bool valid(Universe *u, int m, int n) {
    return m >= 0 && m < u->m && n >= 0 && n < u->n;
}

int uv_rows(Universe *u) {
    return u->m;
}

int uv_cols(Universe *u) {
    return u->n;
}

void uv_set_cell(Universe *u, int m , int n) {
    if (valid(u, m, n)) {
        u->grid[m][n] = 1;
    }
}

void uv_clr_cell(Universe *u, int m , int n) {
    if (valid(u, m, n)) {
        u->grid[m][n] = 0;
    }
}

int uv_get_cell(Universe *u, int m, int n) {
    return valid(u, m, n) ? u->grid[m][n] : -1;
}

bool uv_populate(Universe *u, FILE *infile) {
    int m = 0, n = 0;
    while (fscanf(infile, "%d %d\n", &m, &n) != -1) {
        if (valid(u, m, n)) {
            u->grid[m][n] = 1;
        } else {
            fprintf(stderr, "Malformed input.\n");
            return false;
        }
    }
    return true;
}

int uv_census(Universe *u, int m, int n, bool toroidal) {
    int count = 0;
    for (int y = -1; y <= 1; y += 1) {
        for (int x = -1; x <= 1; x += 1) {
            if (y || x) {
                int m_ = toroidal ? (m + u->m + y) % u->m : m + y;
                int n_ = toroidal ? (n + u->n + x) % u->n : n + x;
                if (valid(u, m_, n_) && u->grid[m_][n_]) {
                    count += 1;
                }
            }
        }
    }
    return count;
}

void uv_print(Universe *u, FILE *outfile) {
    for (int i = 0; i < u->m; i += 1) {
        for (int j = 0; j < u->n; j += 1) {
            fputc(u->grid[i][j] ? 'o' : '.', outfile);
        }
        fputc('\n', outfile);
    }
}


