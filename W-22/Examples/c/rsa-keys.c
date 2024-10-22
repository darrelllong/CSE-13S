#include <inttypes.h>
#include <stdio.h>

int64_t gcd(int64_t a, int64_t b) {
  while (b != 0) {
    int64_t t = a;
    a = b;
    b = t % b;
  }
  return a;
}

static inline int64_t abs(int64_t x) { return x < 0 ? -x : x; }

int64_t lcm(int64_t a, int64_t b) { return abs(a * b) / gcd(a, b); }

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

  int64_t p = 1781309;       // Random prime
  int64_t q = 1403827;       // Random prime
  int64_t e = (1 << 16) + 1; // Canonical e

  int64_t n = p * q;
  printf("n = %" PRId64 "\n\n", n);

  int64_t totient = (p - 1) * (q - 1); // Euler's 𝜑
  printf("𝜑 = %" PRId64 "\n", totient);
  while (gcd(e, totient) != 1) {
    e += 2;
  }
  printf("p = %" PRId64 ", q = %" PRId64 ", e = %" PRId64 "\n", p, q, e);

  int64_t d = inverse(e, totient);
  printf("d = %" PRId64 "\n", d);
  printf("%" PRId64 " * %" PRId64 " = %" PRId64 "\n", e, d, (e * d) % totient);

  int64_t lambda = lcm(p - 1, q - 1); // Carmichael's 𝛌
  printf("\n𝛌 = %" PRId64 "\n", lambda);
  while (gcd(e, lambda) != 1) {
    e += 2;
  }
  printf("p = %" PRId64 ", q = %" PRId64 ", e = %" PRId64 "\n", p, q, e);

  d = inverse(e, lambda);
  printf("d = %" PRId64 "\n", d);
  printf("%" PRId64 " * %" PRId64 " = %" PRId64 "\n", e, d, (e * d) % lambda);

  return 0;
}
