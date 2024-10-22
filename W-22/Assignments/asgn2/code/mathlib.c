#include "mathlib.h"

double Sin(double x) {
    double t = x; // 1st term.
    double s = x; // Sum starts with 1st term.

    for (double n = 3.0, p = -1; Abs(t) > EPSILON; n += 2.0, p *= -1) {
        t *= (x / n) * (x / (n - 1)); // Next odd term.
        s += p * t; // Add it in.
    }

    return s;
}

double Cos(double x) {
    double t = 1.0; // 0th term.
    double s = 1.0; // Sum starts with 0th term.

    for (double n = 2.0, p = -1; Abs(t) > EPSILON; n += 2.0, p *= -1) {
        t *= (x / n) * (x / (n - 1)); // Next even term.
        s += p * t; // Add it in.
    }

    return s;
}

double Sqrt(double x) {
    double z = 0.0;
    double y = x < 1.0 ? 1.0 : x;

    while (Abs(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + x / z);
    }

    return y;
}

double Exp(double x) {
    double t = 1.0;
    double s = 1.0;

    for (double k = 1.0; Abs(t) > EPSILON; k += 1.0) {
        t *= Abs(x) / k;
        s += t;
    }

    return x < 0.0 ? 1.0 / s : s;
}

double Log(double x) {
    double z = 1.0;
    double y = Exp(z);

    while (Abs(y - z) > EPSILON) {
        z = y;
        y = z - 1 + x / Exp(z);
    }

    return y;
}

//
// Composite Simpson's 1/3 Rule:
//
//  a           h            n/2-1          n/2
// ∫  f(x)dx ≈ --- [f(x_0) + 2 Σ f(x_2j) + 4 Σ f(x_2j - 1) + f(x_n)]
//  b           3             j=1           j=1
//
//  n is assumed to be an even number.
//
double integrate(double (*f)(double x), double a, double b, uint32_t n) {
    double h = (b - a) / n;
    double s = f(a);

    for (uint32_t j = 1; j <= n / 2 - 1; j += 1) {
        s += 2.0 * f(a + 2 * j * h);
    }

    for (uint32_t j = 1; j <= n / 2; j += 1) {
        s += 4.0 * f(a + (2 * j - 1) * h);
    }

    s += f(b);
    s *= h / 3.0;
    return s;
}
