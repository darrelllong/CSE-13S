#include "nv.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint32_t bytes(uint32_t nibble_len) {
  return (nibble_len & 1) ? nibble_len / 2 + 1 : nibble_len / 2;
}

NibbleVector *nv_create(uint32_t nibble_len) {
  NibbleVector *n = (NibbleVector *)malloc(sizeof(NibbleVector));
  assert(n);

  n->length = nibble_len;
  n->v = (uint8_t *)calloc(bytes(n->length), sizeof(uint8_t));
  assert(n->v);

  return n;
}

void nv_delete(NibbleVector *n) {
  free(n->v);
  n->v = NULL;
  free(n);
  n = NULL;
  return;
}

void nv_set_nibble(NibbleVector *n, uint32_t i, uint8_t v) {
  n->v[i / 2] = (n->v[i / 2] & (0xF0 >> (4 * (i & 1)))) | (v << (4 * (i & 1)));
  return;
}

void nv_clr_nibble(NibbleVector *n, uint32_t i) {
  n->v[i / 2] &= (0xF0 >> (4 * (i & 1)));
  return;
}

uint8_t nv_get_nibble(NibbleVector *n, uint32_t i) {
  return (n->v[i / 2] & (0xF << (4 * (i & 1)))) >> (4 * (i & 1));
}

void nv_print(NibbleVector *n) {
  printf("[");

  for (uint32_t i = 0; i < n->length; i += 1) {
    printf("0x%0X", nv_get_nibble(n, i));

    if (i != n->length - 1) {
      printf(", ");
    }
  }

  printf("]\n");
}
