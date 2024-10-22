#include "mathlib.h"

static int terms = 0;

//
//      ∞   1
// e =  ∑  ---
//     k=0  k!
//
double e(void) {
    terms = 1;
    double term = 1.0; // 0th term.
    double sum = term; // Add in 0th term.

    for (double k = 1.0; absolute(term) > EPSILON; k += 1.0, terms += 1) {
        term *= 1.0 / k;
        sum += term;
    }

    return sum;
}

int e_terms(void) {
    return terms;
}
