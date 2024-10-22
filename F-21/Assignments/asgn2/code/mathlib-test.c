#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mathlib.h"
#include "set.h"

void diffconst(char *fn, double f, char *gn, double g) {
    printf("%s = %16.15lf, %s = %16.15lf, diff = %16.15lf\n", fn, f, gn, g, absolute(f - g));
}

void diffvalue(char *fn, double (*f)(double), char *gn, double (*g)(double), double x) {
    printf(
        "%s(%lf) = %16.15lf, %s(%lf) = %16.15lf, diff = %16.15lf\n",
        fn, x, f(x), gn, x, g(x), absolute(f(x) - g(x))
    );
}

#define OPTIONS "aebmwrvnsh"

void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "   A test harness for the small numerical library.\n"
            "\n"
            "USAGE\n"
            "   %s [-aebmrvnsh]\n"
            "\n"
            "OPTIONS\n"
            "  -a   Runs all tests.\n"
            "  -e   Runs e test.\n"
            "  -b   Runs BBP pi test.\n"
            "  -m   Runs Madhava pi test.\n"
            "  -r   Runs Euler pi test.\n"
            "  -v   Runs Viete pi test.\n"
            "  -n   Runs Newton square root tests.\n"
            "  -s   Print verbose statistics.\n"
            "  -h   Display program synopsis and usage.\n",
            exec);
}

int main(int argc, char **argv) {
    struct option options[] = {
        { "all",      no_argument, NULL, 'a' },
        { "e",        no_argument, NULL, 'e' },
        { "bbp",      no_argument, NULL, 'b' },
        { "madhava",  no_argument, NULL, 'm' },
        { "euler",    no_argument, NULL, 'r' },
        { "viete",    no_argument, NULL, 'v' },
        { "newton",   no_argument, NULL, 'n' },
        { "stats",    no_argument, NULL, 's' },
        { "help",     no_argument, NULL, 'h' },
        { NULL, 0, NULL, 0 },
    };

    bool stats = false;
    Set fns = set_empty();
    typedef enum Fn { E, EULER, BBP, MADHAVA, VIETE, NEWTON } Fn;

    int opt = 0;
    while ((opt = getopt_long(argc, argv, OPTIONS, options, NULL)) != -1) {
        switch (opt) {
        case 's': { stats = true; break; }
        case 'a': { fns = set_complement(set_empty()); break; }
        case 'e': { fns = set_insert(fns, E);          break; }
        case 'r': { fns = set_insert(fns, EULER);      break; }
        case 'b': { fns = set_insert(fns, BBP);        break; }
        case 'm': { fns = set_insert(fns, MADHAVA);    break; }
        case 'v': { fns = set_insert(fns, VIETE);      break; }
        case 'n': { fns = set_insert(fns, NEWTON);     break; }
        case 'h': { usage(argv[0]); return EXIT_SUCCESS; }
        default:  { usage(argv[0]); return EXIT_FAILURE; }
        }
    }

    if (fns == set_empty()) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    for (Fn f = E; f <= NEWTON; f += 1) {
        if (set_member(fns, f)) {
            switch (f) {
            case E:
                diffconst("e()", e(), "M_E", M_E);
                if (stats) printf("e() terms = %d\n", e_terms());
                break;
            case EULER:
                diffconst("pi_euler()", pi_euler(), "M_PI", M_PI);
                if (stats) printf("pi_euler() terms = %d\n", pi_euler_terms());
                break;
            case BBP:
                diffconst("pi_bbp()", pi_bbp(), "M_PI", M_PI);
                if (stats) printf("pi_bbp() terms = %d\n", pi_bbp_terms());
                break;
            case MADHAVA:
                diffconst("pi_madhava()", pi_madhava(), "M_PI", M_PI);
                if (stats) printf("pi_madhava() terms = %d\n", pi_madhava_terms());
                break;
            case VIETE:
                diffconst("pi_viete()", pi_viete(), "M_PI", M_PI);
                if (stats) printf("pi_viete() terms = %d\n", pi_viete_factors());
                break;
            case NEWTON:
                for (double x = 0.0; x < 10.0; x += 0.1) {
                    diffvalue("sqrt_newton", sqrt_newton, "sqrt", sqrt, x);
                    if (stats) printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
                }
                break;
            }

        }
    }

    return EXIT_SUCCESS;
}
