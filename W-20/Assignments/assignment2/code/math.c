#include "sin.h"
#include "cos.h"
#include "tan.h"
#include "exp.h"

#include <math.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define DELTA     0.001
#define OPTIONS   "ascte"

typedef enum funcs {
  SIN = 0x1, COS = 0x2, TAN = 0x4, EXP = 0x8, ALL = 0xF
} funcs;

void print_usage(char **argv) {
  fprintf(stderr, "Program usage: %s -[ascte]\n"
    "  -a: runs all functions (sin, cos, tan, exp)\n"
    "  -s: runs sin function\n"
    "  -c: runs cos function\n"
    "  -t: runs tan function\n"
    "  -e: runs exp function\n",
    argv[0]);
  return;
}

void print_diffs(char *func, double (*f)(double x), double (*g)(double x),
  double lower, double upper, double inc) {
  printf("x        %s             Library         Difference\n", func);
  printf("-        ---             -------         ----------\n");

  for (double r = lower; r <= upper; r += inc) {
    printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n",
      r, f(r), g(r), f(r) - g(r));
  }

  return;
}

void print_ratio(char *func, double (*f)(double x), double (*g)(double x),
  double lower, double upper, double inc) {
  printf("x        %s             Library         Ratio\n", func);
  printf("-        ---             -------         -----\n");

  for (double r = lower; r <= upper; r += inc) {
    printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n",
      r, f(r), g(r), f(r) / g(r));
  }

  return;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    print_usage(argv);
    return -1;
  }

  int c = 0;
  uint8_t func_set = 0;

  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'a':   { func_set |= ALL;   break; }
    case 's':   { func_set |= SIN;   break; }
    case 'c':   { func_set |= COS;   break; }
    case 't':   { func_set |= TAN;   break; }
    case 'e':   { func_set |= EXP;   break; }
    default:    { print_usage(argv); return -1; }
    }
  }

  for (uint8_t func = SIN; func <= EXP; func <<= 1) {
    if (func_set & func) {
      switch (func) {
      case SIN:
        print_diffs("Sin", Sin, sin, -2.0 * M_PI, 2.0 * M_PI, M_PI / 16);
        break;
      case COS:
        print_diffs("Cos", Cos, cos, -2.0 * M_PI, 2.0 * M_PI, M_PI / 16);
        break;
      case TAN:
        print_diffs("Tan", Tan, tan, -0.5 * M_PI + DELTA, 0.5 * M_PI - DELTA,
            M_PI / 16);
        break;
      case EXP:
        print_ratio("Exp", Exp, exp, 0.0, 50.0, 0.5);
        break;
      }
    }
  }

  return 0;
}
