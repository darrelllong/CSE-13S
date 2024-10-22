#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#define SMALL   30
#define MAXIMUM 1000
#define OPTIONS "Spws:n:"

typedef enum { PRIME, PERFECT, ABUNDANT, DEFICIENT, WEIRD } class;

uint64_t sum(uint64_t *v, uint64_t c) {
  uint64_t s = 0;
  for (uint64_t i = 0; i < c; i += 1) {
    s += v[i];
  }
  return s;
}

class whatIs(uint64_t n, uint64_t *v, uint64_t c) {
  uint64_t s = sum(v, c);
  if      (s == 1) { return PRIME;     } // Only 1 as a divisor
  else if (s == n) { return PERFECT;   } // ∑ proper divisors = k
  else if (s < n)  { return DEFICIENT; } // ∑ proper divisors < k
  else             { return ABUNDANT;  } // ∑ proper divisors > k
}

bool isSemiperfect(uint64_t n, uint64_t *v, uint64_t c) {
  for (uint64_t i = 0; i < (uint64_t) 1 << c; i += 1) { // For each subset, 𝒫(n)
    uint64_t s = 0;
    for (uint64_t j = 0; j < c; j += 1) { // Try a subset
      if (i & (uint64_t) 1 << j) {
        s += v[j];
      }
    }
    if (s == n) { return true; } // This subset sums to n
  }
  return false;
}

bool isWeird(uint64_t n, uint64_t *v, uint64_t c) {
  return whatIs(n, v, c) == ABUNDANT && !isSemiperfect(n, v, c);
}

void printDivisors(uint64_t *v, uint64_t c) {
  printf("[1");
  for (uint64_t i = 1; i < c; i += 1) {
    printf(", %" PRIu64, v[i]);
  }
  printf("]");
}

int main(int argc, char **argv) {
  uint64_t n = MAXIMUM, s = 2;
  bool imperfect = true, weirdness = false, small = false;
  int opt;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'p':
      imperfect = !imperfect; // Toggle
      break;
    case 'w':
      weirdness = !weirdness; // Toggle
      break;
    case 'S':
      small = !small; // Toggle
      break;
    case 's':
      s = strtoul(optarg, NULL, 10);
      break;
    case 'n':
      n = strtoul(optarg, NULL, 10);
      break;
    }
  }

  for (uint64_t i = s; i <= n; i += 1) {
    uint64_t count = 0, divisors[MAXIMUM];

    for (uint64_t j = 1; j < i; j += 1) { // Find all proper divisors
      if (i % j == 0) {
        divisors[count++] = j;
      }
    }

    if (small && count > SMALL) { // Too big to process in a reasonable time
        continue;
    }

    class kind = whatIs(i, divisors, count);

    if (kind == PERFECT) {
      printf("%" PRIu64 " {%" PRIu64 "}: ", i, count);
      printDivisors(divisors, count);
      printf(", perfect\n");
    } else if (imperfect) {
      printf("%" PRIu64 " {%" PRIu64 "}: ", i, count);
      printDivisors(divisors, count);
      switch (kind) {
      case PRIME:
        printf(", prime\n");
        break;
      case DEFICIENT:
        printf(", deficient\n");
        break;
      case ABUNDANT:
        printf("%s", weirdness && isWeird(i, divisors, count)
                         ? ", abundant and weird\n"
                         : ", abundant\n");
        break;
      default: break; // This should never happen
      }
    }
  }
    return 0;
}
