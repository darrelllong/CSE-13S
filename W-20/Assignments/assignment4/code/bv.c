// bv.c - Contains the function definitions for a BitVector ADT.

#include "bv.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define byte_length(x) ((x / 8) + !!(x % 8))

//
// Creates a new BitVector of specified length.
//
// bit_len : The length in bits of the BitVector.
//
BitVector *bv_create(uint32_t bit_len) {
  BitVector *v = calloc(1, sizeof(BitVector));
  assert(v);
  v->length = bit_len;
  v->vector = calloc(byte_length(bit_len), sizeof(uint8_t));
  assert(v->vector);
  return v;
}

//
// Frees memory allocated for a BitVector.
//
// v : The BitVector.
//
void bv_delete(BitVector *v) {
  free(v->vector);
  free(v);
  v = NULL;
  return;
}

//
// Returns the length in bits of the BitVector.
//
// v : The BitVector.
//
uint32_t bv_get_len(BitVector *v) {
  return v->length;
}

//
// Sets the bit at index in the BitVector.
//
// v : The BitVector.
// i : Index of the bit to set.
//
void bv_set_bit(BitVector *v, uint32_t i) {
  v->vector[i / 8] |= (0x1 << (i % 8));
}

//
// Clears the bit at index in the BitVector.
//
// v : The BitVector.
// i : Index of the bit to clear.
//
void bv_clr_bit(BitVector *v, uint32_t i) {
  v->vector[i / 8] &= ~(0x1 << (i % 8));
}

//
// Gets a bit from a BitVector.
//
// v : The BitVector.
// i : Index of the bit to get.
//
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
  return (v->vector[i / 8] >> (i % 8) & 0x1);
}

//
// Sets all bits in a BitVector.
//
// v : The BitVector.
//
void bv_set_all_bits(BitVector *v) {
  memset(v->vector, 0xff, byte_length((v->length)));
}
