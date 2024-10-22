#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include "usage.h"

#define BILLION (1 << 25)
#define KILO    (1 << 10)

double hickoryDickory() {
  struct timeval t;
  gettimeofday(&t, (struct timezone *)0);
  return t.tv_sec + t.tv_usec / 1000000.0;
}

double walkMemory(uint64_t *m, uint64_t n) {
  double start = hickoryDickory();
  uint64_t a, b;
  for (uint64_t i = 0; i < BILLION; i += 1) {
    a = random() % n; b = random() % n;
    m[a] = m[b];
  }
  return hickoryDickory() - start;
}

int main(void) {
  srandom(time(NULL));
  uint64_t s = KILO;
  uint64_t *m = NULL;
  while ((m = malloc(s * sizeof(uint64_t))) != NULL) {
    printf("%15" PRIu64 "\t%7.4lf\n", s, walkMemory(m, s));
    s *= 2;
    free(m);
  }
  printUsage();
  return 0;
}
