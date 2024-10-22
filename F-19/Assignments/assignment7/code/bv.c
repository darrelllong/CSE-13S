#include "bv.h"
#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
  uint8_t *vector;
  uint32_t length;
};

static inline uint32_t bytes(uint32_t bits) {
  return (bits % 8 == 0) ? bits / 8 : bits / 8 + 1;
}

BitVector *bv_create(uint32_t length) {
  BitVector *b = (BitVector *)malloc(sizeof(BitVector));
  assert_ptr(b, "BitVector not allocated!\n");

  b->length = length;
  b->vector = (uint8_t *)calloc(bytes(b->length), sizeof(uint8_t));
  assert_ptr(b->vector, "BitVector array not allocated!\n");

  return b;
}

void bv_delete(BitVector *b) {
  free(b->vector);
  b->vector = NULL;
  free(b);
  b = NULL;
  return;
}

uint32_t bv_length(BitVector *b) {
  return b->length;
}

void bv_set_bit(BitVector *b, uint32_t i) {
  b->vector[i / 8] |= (1 << (i % 8));
  return;
}

void bv_clr_bit(BitVector *b, uint32_t i) {
  b->vector[i / 8] &= ~(1 << (i % 8));
  return;
}

uint8_t bv_get_bit(BitVector *b, uint32_t i) {
  return (b->vector[i / 8] & (1 << (i % 8))) >> (i % 8);
}

void bv_print(BitVector *b) {
  printf("[");

  for (uint32_t i = 0; i < b->length; ++i) {
    printf("%d", bv_get_bit(b, i));

    if (i != b->length - 1) {
      printf(", ");
    }
  }

  printf("]\n");
  return;
}
