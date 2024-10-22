#include "functions.h"
#include "mathlib.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "abcdefghijn:p:q:stH"

typedef enum { A, B, C, D, E, F, G, H, I, J, TESTS, NONE } TestEnum;

typedef struct {
    char *title;
    double (*f)(double);
    double low;
    double high;
} Test;

static Test tests[TESTS] = {
    { "sqrt(1 - x^4)", a, 0, 1.0 },
    { "1/log(x)", b, 1.01, 2.0 },
    { "exp(-x^2)", c, -10.0, 10.0 },
    { "sin(x^2)", d, -M_PI, M_PI },
    { "cos(x^2)", e, -M_PI, M_PI },
    { "log(log(x))", f, 1.01, 10.0 },
    { "sin(x)/x", g, -M_PI, M_PI },
    { "exp(-x)/x", h, 1.0, 10.0 },
    { "exp(exp(x))", i, 0, 1.0 },
    { "sqrt(sin^2(x) + cos^2(x))", j, 0, M_PI },
};

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Performs numerical integrations using composite Simpson's 1/3 "
        "rule.\n"
        "\n"
        "USAGE\n"
        "   %s OPTIONS\n"
        "\n"
        "OPTIONS\n"
        "  -a               %s\n"
        "  -b               %s\n"
        "  -c               %s\n"
        "  -d               %s\n"
        "  -e               %s\n"
        "  -f               %s\n"
        "  -g               %s\n"
        "  -h               %s\n"
        "  -i               %s\n"
        "  -j               %s\n"
        "  -n partitions    Maximum number of partitions.\n"
        "  -p low           Set low end of integral interval.\n"
        "  -q high          Set high end of integral interval.\n"
        "  -H               Display program synopsis and usage.\n",
        exec, tests[A].title, tests[B].title, tests[C].title, tests[D].title, tests[E].title,
        tests[F].title, tests[G].title, tests[H].title, tests[I].title, tests[J].title);
}

int main(int argc, char **argv) {
    double p = 0.0; // Start of interval.
    double q = 0.0; // End of interval.
    uint32_t n = 100;
    TestEnum t = NONE;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': t = A; break;
        case 'b': t = B; break;
        case 'c': t = C; break;
        case 'd': t = D; break;
        case 'e': t = E; break;
        case 'f': t = F; break;
        case 'g': t = G; break;
        case 'h': t = H; break;
        case 'i': t = I; break;
        case 'j': t = J; break;
        case 'n': n = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'p': p = strtod(optarg, NULL); break;
        case 'q': q = strtod(optarg, NULL); break;
        case 'H': usage(argv[0]); return EXIT_SUCCESS;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    if (t == NONE) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    printf("%s,%lf,%lf,%" PRIu32 "\n", tests[t].title, p, q, n);
    for (uint32_t i = 2; i <= n; i += 2) {
        printf("%" PRIu32 ",%.15lf\n", i, integrate(tests[t].f, p, q, i));
    }

    return EXIT_SUCCESS;
}
