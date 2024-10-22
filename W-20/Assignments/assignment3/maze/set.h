#ifndef _SET_DL
#define _SET_DL

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t set;

static const uint32_t mask = 0x00000003f; // Sets are numbered bits 0 through 63

static inline bool memberSet(uint32_t x, set s) {
  return (s & (0x1 << (x & mask)));
}

static inline set insertSet(uint32_t x, set s) {
  return (s | (0x1 << (x & mask)));
}

static inline set deleteSet(uint32_t x, set s) {
  return (s & ~(0x1 << (x & mask)));
}

static inline set unionSet(set s, set t) { return (s | t); }

static inline set intersectSet(set s, set t) { return (s & t); }

static inline set differenceSet(set s, set t) { return (s & ~t); }

#endif
