#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static inline uint64_t uniform(void) {
    return
    (((uint64_t)random() & 0x01) << 63) |
    (((uint64_t)random() & 0x01) << 31) |
    ((uint64_t)random() << 32) |
    (uint64_t)random();

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

int64_t random_prime(int64_t b) {
  uint64_t r;
  do {
    r = uniform() & ((1 << b) - 1);
  } while (!is_prime(r, 50));
  return r;
}

int64_t gcd(int64_t a, int64_t b) {
  while (b != 0) {
    int64_t t = a;
    a = b;
    b = t % b;
  }
  return a;
}

int64_t lcm(int64_t a, int64_t b) { return llabs(a * b) / gcd(a, b); }

int64_t inverse(int64_t a, int64_t n) {
  int64_t r = n, rP = a;
  int64_t t = 0, tP = 1;
  while (rP != 0) {
    int64_t q = r / rP;
    int64_t tmp = rP;
    rP = r - q * rP;
    r = tmp;
    tmp = tP;
    tP = t - q * tP;
    t = tmp;
  }
  if (r > 1) {
    return 0;
  } else {
    return t < 0 ? t + n : t;
  }
}

int main(void) {

  srandom(getpid());

  int64_t p = random_prime(24); // Random prime
  int64_t q = random_prime(24); // Random prime
  int64_t n = p * q;
  printf("Random RSA keys\n");
  printf("p = %" PRId64 ", q = %" PRId64 ", n = %" PRId64 "\n\n", p, q, n);

  int64_t totient = (p - 1) * (q - 1); // Euler's 𝜑
  printf("𝜑 = %" PRId64, totient);
  int64_t e = (1 << 16) + 1; // Canonical e
  while (gcd(e, totient) != 1) {
    e += 2;
  }
  printf(", e = %" PRId64, e);

  int64_t d = inverse(e, totient);
  printf(", d = %" PRId64 "\n", d);
  printf("%" PRId64 " * %" PRId64 " = %" PRId64 " (mod %" PRId64 ")\n", e, d, (e * d) % totient, totient);

  int64_t lambda = lcm(p - 1, q - 1); // Carmichael's 𝛌
  printf("\n𝛌 = %" PRId64, lambda);
  e = (1 << 16) + 1; // Canonical e
  while (gcd(e, lambda) != 1) {
    e += 2;
  }
  printf(", e = %" PRId64, e);

  d = inverse(e, lambda);
  printf(", d = %" PRId64 "\n", d);
  printf("%" PRId64 " * %" PRId64 " = %" PRId64 " (mod %" PRId64 ")\n", e, d, (e * d) % lambda, lambda);

  printf("\n𝜑/𝛌 = %" PRId64 "/%" PRId64 " = %" PRId64 "\n", totient, lambda, totient/lambda);

  return 0;
}
