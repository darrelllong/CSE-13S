#include <math.h>
#include <stdio.h>

static inline double factorial(int n) {
    double v = 1.0;
    for (int i = 1; i <= n; i += 1) {
        v *= i;
    }
    return v;
}

// A few minor sins are committed in this code:
//
// i.  Using pow() could be replaced by multiplies.
// ii. Using factorial() could be replaced by multiplies as well.
//
// Why is this not bad? Ramanujan's formula converges so rapidly that it doesn't really
// matter. Both of these optimizations require a little algebra.
//
// Consider pow(396, 4 * k), let factor = 396 * 396; factor *= factor:
//
// product  = 1.0    (k = 0)
// product *= factor (k = 4)
// product *= factor (k = 8 = 4 + 4)
// product *= factor (k = 12 = 8 + 4)
// ...

double ramanujan_π(int n) {
    double one_over_π = 0.0;
    for (int k = 0; k < n; k += 1) {
        one_over_π += (2 * sqrt(2) / 9801) * (factorial(4 * k) * (1103 + 26390 * k)) /
                    (pow(factorial(k), 4) * pow(396, 4 * k));
    }
    return 1.0 / one_over_π;
}

#define EPSILON 1e-14

int main(void) {
    double π = 0.0;
    for (int i = 1; fabsl(π - M_PI) > EPSILON; i += 1) {
        π = ramanujan_π(i);
        printf("π(%d) = %33.31lf\t(Δ = %33.32lf)\n", i, π, fabs(π - M_PI));
    }
    return 0;
}
