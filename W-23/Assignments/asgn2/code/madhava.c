#include "mathlib.h"

static int terms = 0;

//
//             n   (-3)^-k
// p(n) = √12  ∑  ---------
//            k=0   2k + 1
//
double pi_madhava(void) {
    terms = 1;
    double term = 1.0;  // 0th term
    double sum = term;  // Add in 0th term.
    double sign = -1.0; // Tracks the sign.
    double coeff = 1.0; // Tracks the 3^k.

    for (double k = 1.0; absolute(term) > EPSILON; k += 1.0, terms += 1) {
        coeff *= 3.0;
        term = sign / (coeff * (2 * k + 1));
        sum += term;
        sign = -sign;
    }

    return sqrt_newton(12) * sum;
}

int pi_madhava_terms(void) {
    return terms;
}
