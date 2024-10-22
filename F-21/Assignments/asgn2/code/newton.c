#include "mathlib.h"

static int iters = 0;

//
//                 f(z_k)
// z_k+1  = z_k - ---------
//                 f'(z_k)
//
double sqrt_newton(double x) {
    iters = 0;
    double z = 0.0;
    double y = x < 1 ? 1 : x;

    while (absolute(y - z) > EPSILON && ++iters) {
        z = y;
        y = 0.5 * (z + x / z);
    }

    return y;
}

int sqrt_newton_iters(void) {
    return iters;
}
