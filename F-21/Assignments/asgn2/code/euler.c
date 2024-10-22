#include "mathlib.h"

static int terms = 0;

//
//              n    1
// p(n) = √ (6  ∑  -----)
//             k=1  k^2
//
double pi_euler(void) {
    terms = 1;
    double term = 1.0; // 1st term.
    double sum = term; // Add in 1st term.

    for (double k = 2.0; absolute(term) > EPSILON; k += 1.0, terms += 1) {
        term = 1.0 / (k * k);
        sum += term;
    }

    return sqrt_newton(6.0 * sum);
}

int pi_euler_terms(void) {
    return terms;
}
