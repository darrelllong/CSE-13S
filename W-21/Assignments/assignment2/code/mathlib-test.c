#include "mathlib.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "asctel"
#define STEP_SZ	0.1

void usage(char *program) {
    fprintf(stderr, "Program usage: %s -[%s]\n"
        "  -a           runs all tests (sin, cos, tan, exp, log)\n"
        "  -s           runs sin tests\n"
        "  -c           runs cos tests\n"
        "  -t           runs tan tests\n"
        "  -e           runs exp tests\n"
        "  -l           runs log tests\n",
        program, OPTIONS);
    return;
}

void diff(char *func, double (*f)(double x), double (*g)(double x), double
        lower, double upper, double inc) {
    printf("  x            %s              Library        Difference\n", func);
    printf("  -            ---              -------        ----------\n");

    for (double r = lower; r <= upper; r += inc) {
        printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n",
                r, f(r), g(r), f(r) - g(r));
    }
    return;
}

typedef enum func {
    NIL = 0x00,
    SIN = 0x01,
    COS = 0x02,
    TAN = 0x04,
    EXP = 0x08,
    LOG = 0x10,
    ALL = 0xFF
} func;

int main(int argc, char **argv) {
    int c = 0;
    func set = NIL;
    while ((c = getopt(argc, argv, OPTIONS)) != -1) {
        switch (c) {
        case 'a': { set |= ALL; break; }
        case 's': { set |= SIN; break; }
        case 'c': { set |= COS; break; }
        case 't': { set |= TAN; break; }
        case 'e': { set |= EXP; break; }
        case 'l': { set |= LOG; break; }
        default:  { usage(argv[0]); return 1; }
        }
    }

    if (set == NIL) {
        usage(argv[0]);
        return 1;
    }

    for (func f = SIN; f <= LOG; f <<= 1) {
        if (set & f) {
            switch (f) {
            case SIN:
                diff("Sin", Sin, sin, -2.0 * M_PI, 2.0 * M_PI, STEP_SZ);
                break;
            case COS:
                diff("Cos", Cos, cos, -2.0 * M_PI, 2.0 * M_PI, STEP_SZ);
                break;
            case TAN:
                diff("Tan", Tan, tan, -M_PI / 3.0, M_PI / 3.0, STEP_SZ);
                break;
            case EXP:
                diff("Exp", Exp, exp, 1.0, 10.0, STEP_SZ);
                break;
            case LOG:
                diff("Log", Log, log, 1.0, 10.0, STEP_SZ);
                break;
            default:
                break;
            }
        }
    }
    return 0;
}
