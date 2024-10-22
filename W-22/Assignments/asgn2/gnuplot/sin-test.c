#include <math.h>
#include <stdio.h>

#define EPSILON 1e-14

static inline double absolute(double x) {
    return x < 0 ? -x : x;
}

double sine(double x) {
    double t = x;
    double s = t; // 1st term.

    for (double n = 3.0, p = -1; absolute(t) > EPSILON; n += 2.0, p *= -1) {
        t *= (x / n) * (x / (n - 1)); // Next odd term.
        s += p * t; // Add it in.
    }

    return s;
}

int main(void) {
    for (double x = -4.0 * M_PI; x < 4.0 * M_PI; x += 0.1) {
        printf("sine(%lf) = %16.15lf, sin(%lf) = %16.15lf, diff = %16.15lf\n",
                x, sine(x), x, sin(x), absolute(sine(x) - sin(x)));
    }
}
