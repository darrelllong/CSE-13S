#ifndef _BLOOM_DL
#define _BLOOM_DL

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "bv.h"
#include "hash.h"

typedef struct bloomFilter {
  uint64_t primary  [2]; // Provide for three different hash functions
  uint64_t secondary[2];
  uint64_t tertiary [2];
  bitV *f;
} bloom;

static inline bloom *newBF(uint32_t size) {
  bloom *b = (bloom *) malloc(sizeof(bloom));
  if (b) {
    b->primary  [0] = 0xfc28ca6885711cf7; // U.S. Consitution
    b->primary  [1] = 0x2841af568222f773;
    b->secondary[0] = 0x85ae998311115ae3; // Il nome della rosa
    b->secondary[1] = 0xb6fac2ae33a40089;
    b->tertiary [0] = 0xd37b01df0ae8f8d0; // The Cremation of Sam McGee
    b->tertiary [1] = 0x911d454886ca7cf7;
    b->f            = newVec(size);
    return b;
  }
  else {
    return (bloom *) NIL;
  }
}

static inline void destroyBF(bloom *b) { delVec(b->f); free(b); return; }

static inline void insertBF(bloom *b, char *key) {
  setBit(b->f, keyedHash(key, strlen(key), b->primary  ) % lenVec(b->f));
  setBit(b->f, keyedHash(key, strlen(key), b->secondary) % lenVec(b->f));
  setBit(b->f, keyedHash(key, strlen(key), b->tertiary ) % lenVec(b->f));
  return;
}

static inline bool probeBF(bloom *b, char *key) {
  return (valBit(b->f, keyedHash(key, strlen(key), b->primary  ) % lenVec(b->f)) &&
          valBit(b->f, keyedHash(key, strlen(key), b->secondary) % lenVec(b->f)) &&
          valBit(b->f, keyedHash(key, strlen(key), b->tertiary ) % lenVec(b->f)));
}

#endif
