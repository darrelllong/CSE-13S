#include "mathlib.h"
#include "set.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "asctl"

void usage(char *program) {
    fprintf(stderr,
        "Program usage: %s -[%s]\n"
        "  -a   Runs all tests (arcsin, arccos, arctan, log)\n"
        "  -s   Runs arcsin tests\n"
        "  -c   Runs arccos tests\n"
        "  -t   Runs arctan tests\n"
        "  -l   Runs log tests\n",
        program, OPTIONS);
    return;
}

void diff(char *func, double (*f)(double x), double (*g)(double x), double
        lower, double upper, double inc) {
    printf("  x            %s           Library        Difference\n", func);
    printf("  -            ------           -------        ----------\n");

    for (double r = lower; r <= upper; r += inc) {
        printf(" %7.4lf % 16.8lf % 16.8lf % 16.10lf\n", r, f(r), g(r), f(r) - g(r));
    }
}

typedef enum func { ARCSIN, ARCCOS, ARCTAN, LOG } func;

int main(int argc, char **argv) {
    int opt = 0;
    Set funcs = set_empty();

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': { funcs = set_complement(set_empty()); break; }
        case 's': { funcs = set_insert(funcs, ARCSIN);   break; }
        case 'c': { funcs = set_insert(funcs, ARCCOS);   break; }
        case 't': { funcs = set_insert(funcs, ARCTAN);   break; }
        case 'l': { funcs = set_insert(funcs, LOG);      break; }
        default:  { usage(argv[0]); return 1; }
        }
    }

    if (funcs == set_empty()) {
        usage(argv[0]);
        return 1;
    }

    for (func f = ARCSIN; f <= LOG; f += 1) {
        if (set_member(funcs, f)) {
            switch (f) {
            case ARCSIN: { diff("arcSin", arcSin, asin, -1.0, 1.0, 0.1); break; }
            case ARCCOS: { diff("arcCos", arcCos, acos, -1.0, 1.0, 0.1); break; }
            case ARCTAN: { diff("arcTan", arcTan, atan, 1.0, 10.0, 0.1); break; }
            case LOG:    { diff("Log   ", Log,    log,  1.0, 10.0, 0.1); break; }
            }
        }
    }

    return 0;
}
