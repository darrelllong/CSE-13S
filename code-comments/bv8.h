#pragma once

// BSD License
#include "LICENSE.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define BITS_PER_UNIT 8

typedef struct bitV {
  uint8_t *v;
  uint32_t l;
} bitV;

static inline bitV *newVec(uint32_t l) {
  bitV *v = (bitV *) malloc(sizeof(bitV));
  if (v) {
    uint32_t bytes = l / BITS_PER_UNIT + (l % BITS_PER_UNIT ? 1 : 0);
    v->v = (uint8_t *) calloc(bytes, sizeof(uint8_t));
    v->l = l;
    return v;
  } else {
    return (bitV *) 0;
  }
}

static inline void delVec(bitV *v) {
  if (v && v->v) {
    free(v->v);
  }
  if (v) {
    free(v);
  }
  return;
}

static inline void setBit(bitV *x, uint32_t k) {
  if (x && x->v) {
    x->v[k / BITS_PER_UNIT] |= (0x1 << k % BITS_PER_UNIT);
  }
  return;
}

static inline void clrBit(bitV *x, uint32_t k) {
  if (x && x->v) {
    x->v[k / BITS_PER_UNIT] &= ~(0x1 << k % BITS_PER_UNIT);
  }
  return;
}

static inline bool getBit(bitV *x, uint32_t k) { // We must assume a valid pointer
  return (x->v[k / BITS_PER_UNIT] >> k % BITS_PER_UNIT) & 0x1;
}

static inline uint32_t lenVec(bitV *x) {
  return x ? x->l : 0;
}
