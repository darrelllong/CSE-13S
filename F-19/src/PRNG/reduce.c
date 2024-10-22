#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define EVEN(X) ((0x1 & X) == 0)

typedef __uint128_t uint128_t; // Clang and GCC extension

uint64_t GCD(uint64_t a, uint64_t b) {
  uint64_t d = 0;
  while (EVEN(a) && EVEN(b)) {
    a >>= 1;
    b >>= 1;
    d +=  1;
  }
  while (EVEN(a)) { a >>= 1; }
  do {
    while (EVEN(b)) { b >>= 1; }
    if (a > b) { uint64_t t = a; a = b; b = t; }
    b -= a;
  } while (b != 0);
  return a << d;
}

void reduce(uint64_t *a, uint64_t *b) {
  uint64_t g = 1;
  while ((g = GCD(*a, *b)) != 1) {
    *a /= g;
    *b /= g;
  }
  return;
}

int main(void) {
  uint64_t x, y;
  int r = open("/dev/random", O_RDONLY);
  read(r, &x, 8);
  read(r, &y, 8);
  printf("x = %" PRIu64 "\n", x);
  printf("y = %" PRIu64 "\n", y);
  reduce(&x, &y);
  printf("x = %" PRIu64 "\n", x);
  printf("y = %" PRIu64 "\n", y);
  return 0;
}
