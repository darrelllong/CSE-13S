#ifndef __BV_H__
#define __BV_H__

#include <stdint.h>

typedef struct BitVector BitVector;

BitVector *bv_create(uint32_t length);

void bv_delete(BitVector **v);

uint32_t bv_length(BitVector *v);

void bv_set_bit(BitVector *v, uint32_t i);

void bv_clr_bit(BitVector *v, uint32_t i);

void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit);

uint8_t bv_get_bit(BitVector *v, uint32_t i);

void bv_print(BitVector *v);

#endif
