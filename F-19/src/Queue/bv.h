#ifndef _BV_DL
#define _BV_DL
#include <stdint.h>
#include <stdlib.h>

typedef struct bitV {
  uint8_t *v;
  uint32_t l;
} bitV;

static inline bitV *newVec(uint32_t l) {
  bitV *v = (bitV *) malloc(sizeof(bitV));
  if (v) {
    v->v = (uint8_t *) calloc(l / 8 + (l % 8 != 0 ? 1 : 0), sizeof(uint8_t));
    v->l = l;
    return v;
  } else {
    return (bitV *) 0;
  }
}

static inline void delVec(bitV *v) { free(v->v); free(v); return; }

static inline void setBit(bitV *x, uint32_t k) {
  x->v[k / 8] |= (0x1 << k % 8);
  return;
}

static inline bool valBit(bitV *x, uint32_t k) {
  return (x->v[k / 8] & (0x1 << k % 8)) != 0;
}

static inline uint32_t lenVec(bitV *x) { return x->l; }

#endif
