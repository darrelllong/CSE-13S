#include "hamming.h"

#include "bv.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HAM_MAT_ROW 4
#define HAM_MAT_COL 8
#define HAM_MAT_SZ  (HAM_MAT_COL * HAM_MAT_ROW)

#define HAM_LT_SZ 16

static bitV *generator;
static bitV *parity;

static void ham_print_matrix(bitV *mat) {
    uint8_t i, j;

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = 0; j < HAM_MAT_COL; j++) {
            printf("%u ", bv_get_val(mat, (i * HAM_MAT_COL) + j));
        }
        printf("\n");
    }
}

static void ham_init_parity(bitV *par) {
    uint8_t i, j;

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = 0; j < HAM_MAT_COL / 2; j++) {
            if (i != j)
                bv_set_bit(par, (i * HAM_MAT_COL) + j);
        }
    }

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = HAM_MAT_COL / 2; j < HAM_MAT_COL; j++) {
            if ((j - (HAM_MAT_COL / 2)) == i)
                bv_set_bit(par, (i * HAM_MAT_COL) + j);
        }
    }
}

static void ham_init_gen(bitV *gen) {
    uint8_t i, j;

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = 0; j < HAM_MAT_COL; j++) {
            if (i == j)
                bv_set_bit(gen, (i * HAM_MAT_COL) + j);
        }
    }

    for (i = 0; i < HAM_MAT_ROW; i++) {
        for (j = HAM_MAT_COL / 2; j < HAM_MAT_COL; j++) {
            if ((j - (HAM_MAT_COL / 2)) != i)
                bv_set_bit(gen, (i * HAM_MAT_COL) + j);
        }
    }
}

uint8_t ham_init() {
    generator = bv_new(HAM_MAT_SZ);
    if (generator == NULL) {
        return 1;
    }

    parity = bv_new(HAM_MAT_SZ);
    if (parity == NULL) {
        bv_del(&generator);
        return 1;
    }

    ham_init_gen(generator);
    ham_init_parity(parity);

    return 0;
}

static inline uint8_t get_bit(uint8_t val, uint8_t idx) {
    return (val & (1 << idx)) >> idx;
}

static uint8_t ham_dot_product(bitV *gen, uint8_t val, uint8_t col) {
    uint8_t i;
    uint8_t sum;

    sum = 0;
    for (i = 0; i < HAM_MAT_ROW; i++) {
        sum ^= bv_get_val(gen, (i * HAM_MAT_COL) + col) & get_bit(val, i);
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

static uint8_t ham_dot_product_T(bitV *gen, uint8_t val, uint8_t row) {
    uint8_t i;
    uint8_t sum;

    sum = 0;
    for (i = 0; i < HAM_MAT_COL; i++) {
        sum ^= bv_get_val(gen, (row * HAM_MAT_COL) + i) & get_bit(val, i);
    }

    return sum;
}

ham_rc ham_decode(uint8_t encoded, uint8_t *decoded) {
    uint8_t i;
    uint8_t val = 0;
	// Look up table to determine which bit needs to bit flipped. If one bit flip cannot correct
	// the code then the look up table returns HAM_ERR.
    static uint8_t lt[HAM_LT_SZ] = { 0, 0, 1, HAM_ERR, 2, HAM_ERR, HAM_ERR, 7, 3, HAM_ERR, HAM_ERR,
        6, HAM_ERR, 5, 4, HAM_ERR };

    for (i = 0; i < 4; i++) {
        val |= ham_dot_product_T(generator, encoded, i) << i;
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

void ham_destroy(void) {
    bv_del(&generator);
    bv_del(&parity);
}
