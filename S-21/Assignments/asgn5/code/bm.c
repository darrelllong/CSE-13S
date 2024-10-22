#include "bm.h"
#include "bv.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};

BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *)malloc(sizeof(BitMatrix));
    m->vector    = bv_create(rows * cols);
    m->rows      = rows;
    m->cols      = cols;
    return m;
}

void bm_delete(BitMatrix **m) {
    if (*m) {
        bv_delete(&(*m)->vector);
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
    bv_set_bit(m->vector, r * m->cols + c);
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    bv_clr_bit(m->vector, r * m->cols + c);
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    return bv_get_bit(m->vector, r * m->cols + c);
}

BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    BitMatrix *m = bm_create(1, length);
    for (int i = 0; i < 8; i += 1) {
        if (byte & (1 << i)) {
            bv_set_bit(m->vector, i);
        } else {
            bv_clr_bit(m->vector, i);
        }
    }
    return m;
}

uint8_t bm_to_data(BitMatrix *m) {
    uint8_t data = 0;
    for (int i = 0; i < 8; i += 1) {
        if (bv_get_bit(m->vector, i)) {
            data |= (1 << i);
        }
    }
    return data;
}

BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    BitMatrix *C = bm_create(A->rows, B->cols);
    for (uint32_t i = 0; i < A->rows; i += 1) {
        for (uint32_t j = 0; j < B->cols; j += 1) {
            uint32_t t = (i << 3) + j;
            for (uint32_t k = 0; k < A->cols; k += 1) {
                uint8_t b = bm_get_bit(A, i, k) & bm_get_bit(B, k, j);
                bv_xor_bit(C->vector, t, b);
            }
        }
    }
    return C;
}

void bm_print(BitMatrix *m) {
    printf("┌%*s┐\n", 2 * m->cols + 1, " ");
    for (uint32_t r = 0; r < m->rows; r += 1) {
        printf("│ ");
        for (uint32_t c = 0; c < m->cols; c += 1) {
            printf("%" PRIu8, bm_get_bit(m, r, c));
            if (c + 1 != m->cols) {
                printf(" ");
            }
        }
        printf(" │\n");
    }
    printf("└%*s┘\n", 2 * m->cols + 1, " ");
}
