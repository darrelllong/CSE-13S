#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

#define DIGITS 63

void mantissa(uint64_t d) {
  printf("1/%" PRIu64 " = ", d);
  uint64_t c = 0, n = 1, r;
  do {
    if (n / d) {
      assert(n / d == 1);
      n -= d;
      printf("1");
    } else {
      printf("0");
    }
    r = n % d;
    n <<= 1;
    c += 1;
  } while (r != 0 && c < DIGITS);
  printf("\n");
}

int main(void) {
  for (int i = 1; i < 1000; i += 1) {
    mantissa(i);
  }
  mantissa(2047);
  return 0;
}
