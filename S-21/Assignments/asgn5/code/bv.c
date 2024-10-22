#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

static inline uint32_t bytes(uint32_t bits) {
    return bits % 8 ? bits / 8 + 1 : bits / 8;
}

BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *)malloc(sizeof(BitVector));
    v->length    = length;
    v->vector    = (uint8_t *)calloc(bytes(length), sizeof(uint8_t));
    return v;
}

void bv_delete(BitVector **v) {
    if (*v && (*v)->vector) {
        free((*v)->vector);
        free(*v);
        *v = NULL;
    }
}

uint32_t bv_length(BitVector *v) {
    return v->length;
}

void bv_set_bit(BitVector *v, uint32_t i) {
    v->vector[i / 8] |= (1 << (i % 8));
}

void bv_clr_bit(BitVector *v, uint32_t i) {
    v->vector[i / 8] &= ~(1 << (i % 8));
}

void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    v->vector[i / 8] ^= (bit << (i % 8));
}

uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    return (v->vector[i / 8] >> (i % 8)) & 1;
}
