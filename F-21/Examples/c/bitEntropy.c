#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define BLK 1024

uint64_t gcd(uint64_t a, uint64_t b) {
  while (b != 0) {
    uint64_t t = a;
    a = b;
    b = t % b;
  }
  return a;
}

int main(void) {
  uint64_t count[2] = {0};
  uint8_t block[BLK];
  int bytes;
  // Tally the bits
  while ((bytes = read(0, block, BLK)) > 0) {
    for (int i = 0; i < bytes; i += 1) {
      for (int b = 0; b < 8; b += 1) {
        uint8_t bit = block[i] >> b & 0x1;
        count[bit] += 1;
      }
    }
  }
  // Reduce the fraction
  while (gcd(count[0], count[1]) != 1) {
    uint64_t d = gcd(count[0], count[1]);
    printf("%" PRIu64 "\n", d);
    count[0] /= d;
    count[1] /= d;
  }
  // Calculate entropy
  uint64_t total = count[0] + count[1];
  double z = count[0] / (double)total;
  double o = count[1] / (double)total;
  double e = -(z * log2(z) + o * log2(o));
  printf("entropy = %30.29lf (%" PRIu64 ", %" PRIu64 ")\n", e, count[0], count[1]);
  return 0;
}
