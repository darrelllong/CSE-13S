#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t byte;

typedef uint8_t nibble;

const char *bits[16] = {
    [0] = "0000",  [1] = "0001",  [2] = "0010",  [3] = "0011",
    [4] = "0100",  [5] = "0101",  [6] = "0110",  [7] = "0111",
    [8] = "1000",  [9] = "1001",  [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

static inline nibble upper(byte b) { return (b & 0xf0) >> 4; }

static inline nibble lower(byte b) { return (b & 0x0f); }

static inline byte swap(byte b) { return lower(b) << 4 | upper(b); }

static inline byte parity(byte b) {
  byte p = 0;
  while (b) {
    p ^= (b & 0x1);
    b >>= 1;
  }
  return p;
}

static inline byte dot(byte u, byte v) { return parity(u & v); }

int main(void) {
  for (byte b = 0; b < 0xff; b += 1) {
    printf("%2x parity: %u, lower: %1x, upper: %1x, swapped: %2x\n", b,
           parity(b), upper(b), lower(b), swap(b));
  }

  for (int i = 0; i < 10; i += 1) {
    byte u = random() & 0xff, v = random() & 0xff;
    printf("%s%s . %s%s = %s\n", bits[upper(u)], bits[lower(u)], bits[upper(v)],
           bits[lower(v)], bits[dot(u, v)]);
  }
  return 0;
}
