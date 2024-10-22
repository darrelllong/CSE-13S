#include "bv64.h"

uint64_t parity(bitV *b) {
  uint64_t p = 0;
  uint32_t words = b->l / BITS_PER_UNIT + (b->l % BITS_PER_UNIT ? 1 : 0);
  for (uint32_t i = 0; i < words; i += 1) {
    p ^= b->v[i];
  }
  p = p ^ (p >> 32);
  p = p ^ (p >> 16);
  p = p ^ (p >> 8);
  p = p ^ (p >> 4);
  p = p ^ (p >> 2);
  p = p ^ (p >> 1);
  return p & 1;
}
