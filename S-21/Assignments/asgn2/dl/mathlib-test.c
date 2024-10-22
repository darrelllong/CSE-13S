#include "mathlib.h"

#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef uint16_t set;

typedef enum elm { SIN, COS, TAN, ASIN, ACOS, ATAN, EXP, LOG, POW, SQRT, biSQRT, XYZZY } elem;

static inline set add(set s, uint32_t l) {
    return s | (1 << l);
}

static inline bool member(set s, uint32_t l) {
    return s & (1 << l);
}

#define OPTIONS "asctSCTelrbP"

void usage(char *program) {
    fprintf(stderr,
        "Program usage: %s [-%s]\n"
        "  -a           runs all tests (sin, cos, tan, asin, acos, atan, "
        "exp, log, sqrt)\n"
        "  -s           runs sin tests\n"
        "  -c           runs cos tests\n"
        "  -t           runs tan tests\n"
        "  -S           runs asin tests\n"
        "  -C           runs acos tests\n"
        "  -T           runs atan tests\n"
        "  -e           runs exp tests\n"
        "  -l           runs log tests\n"
        "  -r           runs sqrt tests\n"
        "  -b           runs sqrt tests\n",
        program, OPTIONS);
    return;
}

#define FMT " %7.4lf % 16.8lf % 16.8lf % 30.28lf\n"
#define PLT "% 7lf % 20.18lf\n"

static bool plot = false;

void diff(char *func, double (*f)(double x), double (*g)(double x), double lower, double upper,
    double inc) {
    if (!plot) {
        printf("  x            %6s           Library        Difference\n", func);
        printf("  -            ------           -------        ----------\n");
    }

    for (double r = lower; r <= upper; r += inc) {
        if (plot) {
            printf(PLT, r, f(r) - g(r));
        } else {
            printf(FMT, r, f(r), g(r), f(r) - g(r));
        }
    }
    return;
}

#define STEP_SZ 0.01

int main(int argc, char **argv) {
    int c = 0;
    set options = 0;

    while ((c = getopt(argc, argv, OPTIONS)) != -1) {
        switch (c) {
        case 'P': {
            plot = true;
            break;
        }
        case 'a': {
            for (elem i = SIN; i < XYZZY; i += 1) {
                options = add(options, i);
            }
            break;
        }
        case 's': {
            options = add(options, SIN);
            break;
        }
        case 'c': {
            options = add(options, COS);
            break;
        }
        case 't': {
            options = add(options, TAN);
            break;
        }
        case 'S': {
            options = add(options, ASIN);
            break;
        }
        case 'C': {
            options = add(options, ACOS);
            break;
        }
        case 'T': {
            options = add(options, ATAN);
            break;
        }
        case 'e': {
            options = add(options, EXP);
            break;
        }
        case 'l': {
            options = add(options, LOG);
            break;
        }
        case 'p': {
            options = add(options, POW);
            break;
        }
        case 'r': {
            options = add(options, SQRT);
            break;
        }
        case 'b': {
            options = add(options, biSQRT);
            break;
        }
        default: {
            usage(argv[0]);
            exit(1);
        }
        }
    }

    for (elem i = SIN; i < XYZZY; i += 1) {
        if (member(options, i)) {
            switch (i) {
            case SIN: {
                diff("Sin", Sin, sin, -2.0 * M_PI, 2.0 * M_PI, STEP_SZ);
                break;
            }
            case COS: {
                diff("Cos", Cos, cos, -2.0 * M_PI, 2.0 * M_PI, STEP_SZ);
                break;
            }
            case TAN: {
#define EDGE 0.01
                diff("Tan", Tan, tan, -M_PI / 2.0 + EDGE, M_PI / 2.0 + EDGE, STEP_SZ);
                break;
            }
            case ASIN: {
                diff("ArcSin", arcSin, asin, -1.0, 1.0, STEP_SZ);
                break;
            }
            case ACOS: {
                diff("ArcCos", arcCos, acos, -1.0, 1.0, STEP_SZ);
                break;
            }
            case ATAN: {
                diff("ArcTan", arcTan, atan, -10.0, 10.0, STEP_SZ);
                break;
            }
            case EXP: {
                diff("Exp", Exp, exp, 1.0, 10.0, STEP_SZ);
                break;
            }
            case LOG: {
                diff("Log", Log, log, 1.0, 10.0, STEP_SZ);
                break;
            }
            case POW: {
                break;
            } // Unimplemented
            case SQRT: {
                diff("Sqrt", Sqrt, sqrt, 0.0, 3.0, STEP_SZ);
                break;
            }
            case biSQRT: {
                diff("BSSqrt", biSqrt, sqrt, 0.0, 3.0, STEP_SZ);
                break;
            }
            default: {
                fprintf(stderr, "%s: this should not happen!\n", argv[0]);
                exit(1);
            }
            }
        }
    }

    return 0;
}
