#include "bv64.h"

uint64_t parity(bitV *b) {
  uint64_t p = 0;
  uint32_t words = b->l / BITS_PER_UNIT + (b->l % BITS_PER_UNIT ? 1 : 0);
  for (uint32_t i = 0; i < words; i += 1) {
    p ^= b->v[i];
  }
  p = (p & 0x00000000ffffffff) ^ ((p & 0xffffffff00000000) >> 32);
  p = (p & 0x000000000000ffff) ^ ((p & 0x00000000ffff0000) >> 16);
  p = (p & 0x00000000000000ff) ^ ((p & 0x000000000000ff00) >> 8);
  p = (p & 0x0f) ^ ((p & 0xf0) >> 4);
  p = (p & 0x03) ^ ((p & 0x0c) >> 2);
  return (p & 0x01) ^ ((p & 0x02) >> 1);
}
