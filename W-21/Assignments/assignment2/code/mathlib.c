#include "mathlib.h"

#define EPSILON 1e-14

static inline double abs(double x) {
    return x < 0 ? -x : x;
}

double Sin(double x) {
    double t = x, s = x; // 1st term.
    for (double n = 3.0, p = -1; abs(t) > EPSILON; n += 2.0, p *= -1) {
        t *= (x / n) * (x / (n - 1)); // Next odd term.
        s += p * t; // Add it in.
    }
    return s;
}

double Cos(double x) {
    double t = 1.0, s = 1.0; // 0th term.
    for (double n = 2.0, p = -1; abs(t) > EPSILON; n += 2.0, p *= -1) {
        t *= (x / n) * (x / (n - 1)); // Next even term.
        s += p * t; // Add it in.
    }
    return s;
}

double Tan(double x) {
    return Sin(x) / Cos(x);
}

double Exp(double x) {
    double t = 1.0, s = 1.0; // 0th term.
    for (double n = 1.0; abs(t) > EPSILON; n += 1.0) {
        t = x / n * t; // Next term.
        s += t; // Add it in.
    }
    return s;
}

double Log(double x) {
    double y = 1.0;
    for (double p = Exp(y); abs(p - x) > EPSILON; p = Exp(y)) {
        y = y + (x - p) / p; // Newton iterate for inverse exponentiation
    }
    return y;
}
