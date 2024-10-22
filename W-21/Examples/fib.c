#include <stdio.h>
#include <inttypes.h>

#define MAX 512

static int steps;

uint64_t f(uint64_t n) {
  static uint64_t m[MAX] = {0, 1};
  steps += 1;
  if (n == 0 || n == 1) {
    return n;
#ifdef MEMO
  } else if (m[n]) {
    return m[n];
#endif
  } else {
    return m[n] = f(n - 1) + f(n - 2);
  }
}

int main(void) {
  uint64_t n;
  while (scanf("%" SCNu64, &n) != EOF) {
	steps = 0;
    printf("f(%" PRIu64 ") = %" PRIu64 " (%d steps)\n", n, f(n), steps);
  }
  return 0;
}
