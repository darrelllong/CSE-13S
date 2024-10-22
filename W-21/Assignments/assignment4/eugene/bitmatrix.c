#include "bitmatrix.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    uint8_t *vector;
};

static inline uint32_t bytes(uint32_t bits) {
    return (!bits || bits % 8) ? bits / 8 + 1 : bits / 8;
}

BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix));
    m->vector = (uint8_t *) calloc(bytes(rows * cols), sizeof(uint8_t));
    m->rows = rows;
    m->cols = cols;
    return m;
}

void bm_delete(BitMatrix **m) {
    if (*m) {
        free((*m)->vector);
        (*m)->vector = NULL;
        free(*m);
        *m = NULL;
    }
}

uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t i = (r << 3) + c; m->vector[i >> 3] |= 1 << (i & 7);
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t i = (r << 3) + c; m->vector[i >> 3] &= ~(1 << (i & 7));
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t i = (r << 3) + c; return (m->vector[i >> 3] >> (i & 7)) & 1;
}

BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    BitMatrix *m = bm_create(1, length);
    m->vector[0] = byte;
    return m;
}

uint8_t bm_to_data(BitMatrix *m) {
    return m->vector[0];
}

BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    BitMatrix *C = bm_create(A->rows, B->cols);
    for (uint32_t i = 0; i < A->rows; i += 1) {
        for (uint32_t j = 0; j < B->cols; j += 1) {
            uint32_t t = (i << 3) + j;
            for (uint32_t k = 0; k < A->cols; k += 1) {
                uint8_t b = bm_get_bit(A, i, k) & bm_get_bit(B, k, j);
                C->vector[t >> 3] ^= b << (t & 7);
            }
        }
    }
    return C;
}

void bm_print(BitMatrix *m) {
    printf("┌%*s┐\n", 3 * m->cols, " ");
    for (uint32_t r = 0; r < m->rows; r += 1) {
        printf("│ ");
        for (uint32_t c = 0; c < m->cols; c += 1) {
            printf("%" PRIu8, bm_get_bit(m, r, c));
            if (c + 1 != m->cols) {
                printf(", ");
            }
        }
        printf(" │\n");
    }
    printf("└%*s┘\n", 3 * m->cols, " ");
}
