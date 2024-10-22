#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define OPTIONS "hn:r:"

uint64_t in_circle(double x, double y) {
  double distance = x * x + y * y;
  if (distance <= 1) {
    return 1;
  }
  return 0;
}

void usage(char *exec) {
  fprintf(stderr,
          "SYNOPSIS\n"
          "   Prints the Monte Carlo value of Pi.\n"
          "\n"
          "USAGE\n"
          "   %s [-hn:r:] [-n points] [-r seed]\n"
          "\n"
          "OPTIONS\n"
          "   -h         display program help and usage.\n"
          "   -n points  number of points for estimation.\n"
          "   -r seed    deterministic random stating point.\n",
          exec);
}

int main(int argc, char **argv) {

  uint32_t seed = time(0); // Default (no options) is random based on time.
  srandom(seed);
  uint32_t points = random();

  int opt = 0;
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'n':
      points = (uint32_t)strtoul(optarg, NULL, 10); // Explicit number of points
      break;
    case 'r':
      seed = (uint32_t)strtoul(optarg, NULL,
                               10); // Deterministic seed
      srandom(seed);
      break;
    default:
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  double x, y, pi;
  uint64_t count = 0;
  printf("%10s %8s %16s %16s %6s\n", "Iteration", "Pi", "x", "y", "circle");
  for (uint64_t n = 1; n <= points + 1; n += 1) {
    x = (double)random() / RAND_MAX;
    y = (double)random() / RAND_MAX;
    count += in_circle(x, y);
    pi = (double)count / n * 4;
    printf("%10lu %8g %16g %16g %6lu\n", n - 1, pi, x, y, in_circle(x, y));
  }

  return 0;
}
