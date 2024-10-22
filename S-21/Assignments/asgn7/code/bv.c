#include "bv.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint8_t *vector;
    uint32_t length;
};

static inline uint32_t bytes(uint32_t bits) {
    return bits % 8 ? bits / 8 + 1 : bits / 8;
}

BitVector *bv_create(uint32_t length) {
    BitVector *b = (BitVector *)malloc(sizeof(BitVector));
    if (b) {
        b->length = length;
        b->vector = (uint8_t *)calloc(bytes(b->length), sizeof(uint8_t));
        if (!b->vector) {
            free(b);
            b = NULL;
        }
    }
    return b;
}

void bv_delete(BitVector **b) {
    free((*b)->vector);
    (*b)->vector = NULL;
    free(*b);
    *b = NULL;
}

uint32_t bv_length(BitVector *b) {
    return b->length;
}

void bv_set_bit(BitVector *b, uint32_t i) {
    b->vector[i >> 3] |= (1 << (i & 7));
}

void bv_clr_bit(BitVector *b, uint32_t i) {
    b->vector[i >> 3] &= ~(1 << (i & 7));
}

uint8_t bv_get_bit(BitVector *b, uint32_t i) {
    return (b->vector[i >> 3] >> (i & 7)) & 1;
}

void bv_print(BitVector *b) {
    printf("[");
    for (uint32_t i = 0; i < b->length; ++i) {
        printf("%" PRIu8, bv_get_bit(b, i));
        if (i != b->length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
