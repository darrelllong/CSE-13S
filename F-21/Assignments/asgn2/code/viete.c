#include "mathlib.h"

static int factors = 0;

//
//
//  2     n   a_k
// --- =  Π  -----
//  π    k=1   2
//
// where
//
//  a_1 = √2
//  a_k = √(2 a_(k-1))
//
double pi_viete(void) {
    double prod = 1.0;
    double fact = sqrt_newton(2.0);

    while (absolute(1.0 - (fact / 2.0)) > EPSILON) {
        prod *= fact / 2;
        fact = sqrt_newton(2.0 + fact);
    }

    return 2.0 / prod;
}

int pi_viete_factors(void) {
    return factors;
}
