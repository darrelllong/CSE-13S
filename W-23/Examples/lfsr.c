#include "lfsr.h"

static inline uint64_t parity(uint64_t n) {
  uint32_t p = (n >> 32) ^ (n & 0x00000000ffffffff); // Fold 64 → 32
           p = (p >> 16) ^ (p & 0x000000000000ffff); // Fold 32 → 16
           p = (p >>  8) ^ (p & 0x00000000000000ff); // Fold 16 →  8
           p = (p >>  4) ^ (p & 0x000000000000000f); // Fold  8 →  4
           p = (p >>  2) ^ (p & 0x0000000000000003); // Fold  4 →  2
  return       (p >>  1) ^ (p & 0x0000000000000001); // Fold  2 →  1
}

uint64_t lfsr(uint64_t *l) {
  uint64_t b = *l & 0x1; // Output bit
  *l = (*l >> 1) | (parity(*l & FEEDBACK) << 63); // Perform feedback operation
  return b;
}

uint8_t randomByte(uint64_t *l) {
  uint8_t r = 0;
  for (int i = 0; i < 8; i += 1) { // Collect 8 bits from the LFSR
    if (lfsr(l)) {
      r |= 1 << i;
    }
  }
  return r;
}

void warmLFSR(uint64_t *l) {
  for (int i = 0; i < 256; i += 1) { // Cycle the LFSR into a steady state
    (void) lfsr(l);
  }
}
