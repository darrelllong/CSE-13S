#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static inline uint64_t uniform(void) {
  return (((uint64_t)random() & 0x01) << 63) |
         (((uint64_t)random() & 0x01) << 31) | ((uint64_t)random() << 32) | (uint64_t)random();
}

bool is_even(int64_t n) { return (n & 0x1) == 0; }

bool is_odd(int64_t n) { return (n & 0x1) == 1; }

int64_t power_mod(int64_t a, int64_t d, int64_t n) {
  int64_t v = 1;
  for (int64_t p = a; d > 0; d >>= 1) {
    if (is_odd(d)) {
      v = (v * p) % n;
    }
    p = (p * p) % n;
  }
  return v;
}

int64_t Jacobi(int64_t n, int64_t k) {
  // Compute the Jacobi symbol:
  //   n    ⎡  0 if n ≡ 0 (mod k)
  //  (-) = ⎢  1 if n ≢ 0 (mod k) ⋀ (∃x) a ≡ x**2 (mod k)
  //   k    ⎣ -1 if n ≢ 0 (mod k) ⋀ (∄x) a ≡ x**2 (mod k)
  n = n % k;
  int64_t t = 1;
  while (n != 0) {
    while (is_even(n)) {
      n = n / 2;
      int64_t r = k % 8;
      t = (r == 3 || r == 5) ? -t : t;
    }
    int64_t tmp = k;
    k = n;
    n = tmp;
    t = (n % 4 == 3 && k % 4 == 3) ? -t : t;
    n = n % k;
  }
  return (k == 1) ? t : 0;
}

bool is_prime(int64_t n, int64_t k) {
  if (n < 2 || (n != 2 && n % 2 == 0)) {
    return false;
  }
  if (n == 3) {
    return true;
  }
  for (int i = 0; i < k; i += 1) {
    uint64_t a = (uniform() % (n - 2)) + 2; // a ∊ [2, ..., n - 1]
    int64_t x = Jacobi(a, n);
    if (x == 0 || (power_mod(a, (n - 1) / 2, n) != (x + n) % n)) {
      return false;
    }
  }
  return true;
}

int64_t nextPrime(int64_t *p) {
  if (*p == 2) { *p = 3; return *p; }
  *p |= 0x1;
  do {
    *p += 2;
  }
  while (!is_prime(*p, 50));
  return *p;
}

int main(void) {

  srandom(getpid());
  const int64_t start = 65537;
  for (int64_t p = start; p < start + 1000; nextPrime(&p)) {
    printf("%lld\n", p);
  }
  return 0;
}
