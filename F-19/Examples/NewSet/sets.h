#ifndef _SETS_DL
#define _SETS_DL

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t set;

static const uint32_t mask = 0x00000003f;

static inline bool memberSet(uint32_t x, set s) {
  return ((s & (0x1 << (x & mask))) >> (x & mask));
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
