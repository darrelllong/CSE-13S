#include "hamming.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HAM_MAT_ROW 4
#define HAM_MAT_COL 8

#define HAM_LT_SZ 16

static uint8_t generator[HAM_MAT_ROW];
static uint8_t parity[HAM_MAT_ROW];

static inline uint8_t get_bit(uint8_t val, uint8_t idx) {
    return (val & (1 << idx)) >> idx;
}

static inline uint8_t set_bit(uint8_t val, uint8_t idx) {
    return val | (1 << idx);
}

static void ham_print_matrix(uint8_t *mat) {
    uint8_t i, j;

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = 0; j < HAM_MAT_COL; j++) {
            printf("%u ", get_bit(mat[i], j));
        }
        printf("\n");
    }
}

static void ham_init_parity(uint8_t *par) {
    uint8_t i, j;

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = 0; j < HAM_MAT_COL / 2; j++) {
            if (i != j)
                par[i] = set_bit(par[i], j);
        }
    }

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = HAM_MAT_COL / 2; j < HAM_MAT_COL; j++) {
            if ((j - (HAM_MAT_COL / 2)) == i)
                par[i] = set_bit(par[i], j);
        }
    }
}

static void ham_init_gen(uint8_t *gen) {
    uint8_t i, j;

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = 0; j < HAM_MAT_COL; j++) {
            if (i == j)
                gen[i] = set_bit(gen[i], j);
        }
    }

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = HAM_MAT_COL / 2; j < HAM_MAT_COL; j++) {
            if ((j - (HAM_MAT_COL / 2)) != i)
                gen[i] = set_bit(gen[i], j);
        }
    }
}

uint8_t ham_init() {
    ham_init_gen(generator);
    ham_init_parity(parity);

    return 0;
}

static uint8_t ham_dot_product(uint8_t *gen, uint8_t val, uint8_t col) {
    uint8_t i;
    uint8_t sum;

    sum = 0;
    for (i = 0; i < HAM_MAT_ROW; i++) {
        sum ^= get_bit(gen[i], col) & get_bit(val, i);
    }

    return sum;
}

uint8_t ham_encode(uint8_t val) {
    uint8_t encoded;
    uint8_t i, j;

    encoded = 0;
    val &= 0xF;

    for (i = 0; i < HAM_MAT_COL; i++) {
        encoded |= ham_dot_product(generator, val, i) << i;
    }

    return encoded;
}

static uint8_t ham_dot_product_T(uint8_t *par, uint8_t val, uint8_t row) {
    uint8_t i;
    uint8_t sum;

    sum = 0;
    for (i = 0; i < HAM_MAT_COL; i++) {
        sum ^= get_bit(par[row], i) & get_bit(val, i);
    }

    return sum;
}

ham_rc ham_decode(uint8_t encoded, uint8_t *decoded) {
    uint8_t i;
    uint8_t val = 0;
    // Look up table to determine which bit needs to bit flipped. If one bit flip cannot correct
    // the code then the look up table returns HAM_ERR.
    static uint8_t lt[HAM_LT_SZ] = { 0, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR,
        2, HAM_ERR, 1, 0, HAM_ERR };

    for (i = 0; i < 4; i++) {
        val |= ham_dot_product_T(parity, encoded, i) << i;
    }

    // Error occured. Let's try to fix
    if (val > 0) {
        if (lt[val] == HAM_ERR) {
            return HAM_ERR;
        } else {
            encoded ^= 1 << lt[val];
            *decoded = encoded & 0xF;
            return HAM_ERR_OK;
        }
    } else {
        *decoded = encoded & 0xF;
    }

    return HAM_OK;
}
