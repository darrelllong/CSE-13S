#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define OPTIONS "hvscb:e:S:"

void usage(char *exec) {
  fprintf(stderr,
          "SYNOPSIS\n"
          "   Prints sine(x) or cosine(x).\n"
          "\n"
          "USAGE\n"
          "   %s [-hvscb:e:s:] [-n start] [-r seed]\n"
          "\n"
          "OPTIONS\n"
          "   -h    display program help and usage.\n"
          "   -v    verbose mode.\n"
          "   -s    sine.\n"
          "   -c    cosine.\n"
          "   -b x  begin (default: begin = -2π).\n"
          "   -e x  end (default: end = 2π).\n"
          "   -S x  step used (default: 𝛆 = 0.01).\n",
          exec);
}

int main(int argc, char **argv) {

  double (*f)() = sin;
  char *name = "sin";
  double epsilon = 0.01, begin = -2 * M_PI, end = 2 * M_PI;
  bool verbose = false;

  int opt = 0;
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 's':
      f = sin;
      name = "sin";
      break;
    case 'c':
      f = cos;
      name = "cos";
      break;
    case 'b':
      sscanf(optarg, "%lf", &begin);
      break;
    case 'e':
      sscanf(optarg, "%lf", &end);
      break;
    case 'S':
      sscanf(optarg, "%lf", &epsilon);
      break;
    case 'v':
      verbose = true;
      break;
    default:
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  for (double x = begin; x <= end; x += epsilon) {
    if (verbose) {
      printf("%s(%lf) = %lf\n", name, x, f(x));
    } else {
      printf("%lf\t%lf\n", x, f(x));
    }
  }

  return 0;
}
