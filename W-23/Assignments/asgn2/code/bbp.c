#include "mathlib.h"

static int terms = 0;

//
//         n                  k(120k + 151) + 47
// p(n) =  ∑  16^-k ---------------------------------------
//        k=0        k(k(k(512k + 1024) + 712) + 194) + 15
//
double pi_bbp(void) {
    terms = 1;
    double term = 47.0 / 15.0; // The 0-th term.
    double sum = term;         // Add in the 0th term.
    double coeff = 1.0;        // Builds up value of 16^-k.

    for (double k = 1.0; absolute(term) > EPSILON; k += 1.0, terms += 1) {
        coeff *= 1.0 / 16.0;
        term = coeff * (k * (120 * k + 151) + 47);
        term /= (k * (k * (k * (512 * k + 1024) + 712) + 194) + 15);
        sum += term;
    }

    return sum;
}

int pi_bbp_terms(void) {
    return terms;
}
