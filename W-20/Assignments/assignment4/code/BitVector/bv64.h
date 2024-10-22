#ifndef _BV_64_H
#define _BV_64_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define BITS_PER_UNIT 64

typedef struct bitV {
  uint64_t *v;
  uint32_t l;
} bitV;

static inline uint32_t index(uint32_t i) {
    return !i || i % BITS_PER_UNIT ? i / BITS_PER_UNIT : i / BITS_PER_UNIT - 1;
}

static inline bitV *newVec(uint32_t l) {
  bitV *v = (bitV *) malloc(sizeof(bitV));
  if (v) {
    uint32_t words = l / BITS_PER_UNIT + (l % BITS_PER_UNIT ? 1 : 0);
    v->v = (uint64_t *) calloc(words, sizeof(uint64_t));
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
    x->v[index(k)] |= ((uint64_t) 0x1 << k % BITS_PER_UNIT);
  }
  return;
}

static inline void clrBit(bitV *x, uint32_t k) {
  if (x && x->v) {
    x->v[index(k)] &= ~((uint64_t) 0x1 << k % BITS_PER_UNIT);
  }
  return;
}

static inline bool valBit(bitV *x, uint32_t k) { // We must assume a valid pointer
  return (x->v[index(k)] >> k % BITS_PER_UNIT) & 0x1;
}

static inline uint32_t lenVec(bitV *x) {
  if (x) {
    return x->l;
  } else
    return 0;
}

#endif
