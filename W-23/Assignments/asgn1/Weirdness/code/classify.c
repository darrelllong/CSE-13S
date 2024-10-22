#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXIMUM 1000
#define OPTIONS "pn:"

int main(int argc, char **argv) {
  int n = MAXIMUM;
  bool imperfect = true;
  int opt;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'p':
      imperfect = !imperfect;
      break;
    case 'n':
      n = strtoul(optarg, NULL, 10);
      break;
    }
  }

  for (int i = 2; i <= n; i += 1) {
    char factors[MAXIMUM * MAXIMUM];
    int sum = 1; // Proper divisors always include 1
    snprintf(factors, MAXIMUM, "%d : [1", i);
    for (int j = 2; j < i; j += 1) {
      if (i % j == 0) { // j is a proper divisor of i
        int l = strlen(factors);
        snprintf(factors + l, MAXIMUM - l, ", %d", j);
        sum += j;
        if (sum > i && !imperfect) { break; }
      }
    }
    strcat(factors, "]");

    // A number k is perfect if ∑ proper divisors = k, abundant if ∑ proper
    // divisors > k, and deficient if ∑ proper divisors < k.

    if (sum == i) {
      printf("%s is perfect\n", factors);
    } else if (imperfect && sum == 1) {
      printf("%s is prime\n", factors);
    } else if (imperfect) {
      printf(sum > i ? "%s is abundant\n" : "%s is deficient\n", factors);
    }
  }
  return 0;
}
