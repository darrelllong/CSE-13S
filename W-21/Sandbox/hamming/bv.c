#include "bv.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BV_BITS_SZ            8
#define BV_LEN_BYTES(x)       ((x) / BV_BITS_SZ)
#define BV_BIT(x)             (1 << (x))
#define BV_VALID_IDX(sz, idx) ((idx) < (sz))

typedef struct bitV {
    uint8_t *vector;
    uint32_t length;
} bitV;

static void printVec(bitV *bv) {
    uint32_t i;

    if (bv == NULL)
        return;

    for (i = 0; i < bv->length; i++)
        printf("Bit %u: %u\n", i, bv_get_val(bv, i));
}

bitV *bv_new(uint32_t length) {
    bitV *bv;
    uint32_t len;

    bv = (bitV *) calloc(1, sizeof(bitV));
    if (bv == NULL)
        return NULL;

    // Adding 1 ensures the minimum BitVector length is 1 byte
    len = BV_LEN_BYTES(length) + 1;
    bv->vector = (uint8_t *) calloc(len, sizeof(uint8_t));
    if (bv->vector == NULL) {
        free(bv);
        return NULL;
    }

    bv->length = length;

    return bv;
}

void bv_del(bitV **bv) {
    if (bv == NULL)
        return;

    free((*bv)->vector);

    free(*bv);
    *bv = NULL;
}

uint8_t bv_set_all(bitV *bv) {
    uint32_t i;

    if (bv == NULL)
        return 1;

    for (i = 0; i <= BV_LEN_BYTES(bv->length); i++)
        bv->vector[i] |= ~bv->vector[i];

    return 0;
}

uint8_t bv_set_bit(bitV *bv, uint32_t index) {
    if (bv == NULL || !BV_VALID_IDX(bv->length, index))
        return 1;

    bv->vector[index / BV_BITS_SZ] |= BV_BIT(index % BV_BITS_SZ);

    return 0;
}

uint8_t bv_clr_bit(bitV *bv, uint32_t index) {
    if (bv == NULL || !BV_VALID_IDX(bv->length, index))
        return 1;

    bv->vector[index / BV_BITS_SZ] &= ~BV_BIT(index % BV_BITS_SZ);

    return 0;
}

uint8_t bv_get_val(bitV *bv, uint32_t index) {
    if (bv == NULL || !BV_VALID_IDX(bv->length, index))
        return 0;

    return (bv->vector[index / BV_BITS_SZ] & BV_BIT(index % BV_BITS_SZ)) ? 1 : 0;
}

uint32_t bv_get_len(bitV *bv) {
    if (bv == NULL)
        return 0;

    return bv->length;
}
