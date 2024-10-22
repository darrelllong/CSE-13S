#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bv.h"
#include "sieve.h"

#define BIGGEST 1000

#define LARGEST 67

bool mersenne(uint64_t p) {
  return !((p + 1) & p);
}

static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

char *itoa(int n, uint32_t base) {
  static char b[LARGEST];
  char *t = b + LARGEST;
  bool negative = false;
  if (n < 0) {
    n = -n;
    negative = true;
  }
  *--t = '\0';
  do {
    *--t = digits[n % base];
    n /= base;
  } while (n > 0);
  if (negative) {
    *--t = '-';
  }
  return t;
}

bool palindrome(char *s) {
  char *l = s, *r = s + strlen(s) - 1;
  while (l < r) {
    if (*l++ - *r--) {
      return false;
    }
  }
  return true;
}

uint64_t nextLucas(void) {
  static uint64_t v1 = 2, v2 = 1;
  uint64_t t = v1, v3 = v1 + v2;
  v1 = v2;
  v2 = v3;
  return t;
}

uint64_t nextFibonacci(void) {
  static uint64_t u1 = 1, u2 = 1;
  uint64_t t = u1, u3 = u1 + u2;
  u1 = u2;
  u2 = u3;
  return t;
}

#define OPTIONS "n:sp"

int main(int argc, char **argv) {
  bool primePals = false, special = false;
  uint64_t n = BIGGEST;

  int c;
  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'n':
      n = strtoll(optarg, (char **)NULL, 10);
      break;
    case 'p':
      primePals = true;
      break;
    case 's':
      special = true;
      break;
    default:
      printf("Invalid argument\n");
      break;
    }
  }

  BitVector *V = bv_create(n);
  uint64_t lucas = 2, fibonacci = 1;
  sieve(V);

  if (special) {
    for (uint64_t i = 0; i < n; i += 1) {
      while (i > lucas) {
        lucas = nextLucas();
      }
      while (i > fibonacci) {
        fibonacci = nextFibonacci();
      }
      if (bv_get_bit(V, i)) {
        printf("%" PRIu64 ": prime", i);

        if (mersenne(i)) {
          printf(", mersenne");
        }
        if (i == lucas) {
          printf(", lucas");
        }
        if (i == fibonacci) {
          printf(", fibonacci");
        }

        printf("\n");
      }
    }
  }

  if (primePals) {
    for (uint32_t base = 2; base < 17; base += 1) {
      printf("\nBase %2" PRIu32 "\n", base);
      printf("---- --\n");
      for (uint64_t i = 0; i < n; i += 1) {
        char *s;
        if (bv_get_bit(V, i)) {
          s = itoa(i, base);
          if (palindrome(s)) {
            printf("%" PRIu64 " = %s\n", i, s);
          }
        }
      }
    }
  }

  bv_delete(V);
  return 0;
}
