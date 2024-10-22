#include "trivium.h"
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

static inline uint64_t getBit(uint128_t *n, uint64_t k) {
    return (n->data[k / 64] >> (k % 64)) & 0x1;
}

static inline void setBit(uint128_t *n, uint64_t k) {
    n->data[k / 64] |= ((uint64_t) 0x1 << k % 64);
    return;
}

static inline void clrBit(uint128_t *n, uint64_t k) {
    n->data[k / 64] &= ~((uint64_t) 0x1 << k % 64);
    return;
}

static inline void asgnBit(uint128_t *n, uint64_t k, uint64_t v) {
    if (v) {
        setBit(n, k);
    } else {
        clrBit(n, k);
    }
    return;
}

static inline void loadBits(uint128_t *n, uint64_t low, uint64_t high) {
    n->data[0] = low;
    n->data[1] = high;
    return;
}

static inline void leftShift(uint128_t *n) {
    n->data[1] = n->data[1] << 1 | ((n->data[0] >> 63) & 0x1);
    n->data[0] = n->data[0] << 1;
    return;
}

void startTrivium(state *s, uint128_t key, uint128_t nonce) {
    memset(s, 0, sizeof(*s));
    loadBits(&s->a, key.data[0], key.data[1]);
    loadBits(&s->b, nonce.data[0], nonce.data[1]);
    setBit(&s->c, 108);
    setBit(&s->c, 109);
    setBit(&s->c, 110);
    return;
}

uint64_t Trivium(state *s) {
    uint64_t inA, inB, inC;
    uint64_t outA, outB, outC;
    outA = getBit(&s->a, 65) ^ getBit(&s->a,  92) ^ (getBit(&s->a,  90) & getBit(&s->a,  91));
    outB = getBit(&s->b, 68) ^ getBit(&s->b,  83) ^ (getBit(&s->b,  81) & getBit(&s->b,  82));
    outC = getBit(&s->c, 65) ^ getBit(&s->c, 110) ^ (getBit(&s->c, 108) & getBit(&s->c, 109));
    inA = getBit(&s->a, 68) ^ outC;
    inB = getBit(&s->b, 77) ^ outA;
    inC = getBit(&s->c, 86) ^ outB;
    leftShift(&s->a);
    leftShift(&s->b);
    leftShift(&s->c);
    asgnBit(&s->a, 0, inA);
    asgnBit(&s->b, 0, inB);
    asgnBit(&s->c, 0, inC);
    return outA ^ outB ^ outC;
}

void warmTrivium(state *s) {
    for (int i = 0; i < 1152; i += 1) {
        (void) Trivium(s);
    }
    return;
}

uint64_t randomWord(state *l) {
  uint64_t r = 0;
  for (int i = 0; i < 64; i += 1) { // Collect 64 bits from the LFSR
    if (Trivium(l)) {
      r |= 1 << i;
    }
  }
  return r;
}
