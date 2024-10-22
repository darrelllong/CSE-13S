#include "classify.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_LOWER_LIMIT 2
#define DEFAULT_UPPER_LIMIT 28

void usage(FILE *stream, char *exec) {
    fprintf(stream,
        "SYNOPSIS\n"
        "   A program that classifies numbers [2, n] into the\n"
        "   following categories:\n"
        "    - (i) prime\n"
        "    - (c) composite\n"
        "    - (a) abundant\n"
        "    - (d) deficient\n"
        "    - (p) perfect\n"
        "    - (s) semiperfect\n"
        "    - (w) weird\n"
        "   The proper divisors of the numbers are printed as well.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -n limit    Limit of numbers to classify. [default: 28]\n"
        "   -h          Display this usage message.\n",
        exec
    );
}

void classify(uint32_t n, Vec *factors) {
    printf("%" PRIu32 ": (", n);
    printf(is_prime(factors) ? "i" : "-");
    printf(is_composite(factors) ? "c" : "-");
    printf(is_abundant(n, factors) ? "a" : "-");
    printf(is_deficient(n, factors) ? "d" : "-");
    printf(is_perfect(n, factors) ? "p" : "-");
    printf(is_semiperfect(n, factors) ? "s" : "-");
    printf(is_weird(n, factors) ? "w" : "-");
    printf(") ");
}

int main(int argc, char **argv) {
    uint32_t m = DEFAULT_LOWER_LIMIT;
    uint32_t n = DEFAULT_UPPER_LIMIT;

    int opt;
    while ((opt = getopt(argc, argv, "m:n:h")) != -1) {
        switch (opt) {
        case 'm':
            m = strtoul(optarg, NULL, 10);
            break;
        case 'n':
            n = strtoul(optarg, NULL, 10);
            break;
        case 'h':
            usage(stdout, argv[0]);
            return 0;
        default:
            usage(stderr, argv[0]);
            return 1;
        }
    }

    Vec factors = vec_new();
    for (uint32_t k = m; k <= n; k += 1) {
        vec_clear(&factors);
        factor(k, &factors);
        classify(k, &factors);
        vec_print(&factors);
    }
    vec_drop(&factors);

    return 0;
}
