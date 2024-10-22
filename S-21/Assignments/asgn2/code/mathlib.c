#include "mathlib.h"

#include <math.h>

#define EPSILON 1e-10

static inline double Abs(double x) {
    return x < 0.0 ? -x : x;
}

static double Sqrt(double x) {
    double z = 0.0;
    double y = 1.0;
    while (Abs(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + x / z);
    }
    return y;
}

double arcSin(double x) {
    double z = 0.0;
    double y = x;
    while (Abs(y - z) > EPSILON) {
        z = y;
        y = z - (sin(z) - x) / cos(z);
    }
    return y;
}

double arcCos(double x) {
    double z = 0.0;
    double y = 1.0;
    while (Abs(y - z) > EPSILON) {
        z = y;
        y = z + (cos(z) - x) / sin(z);
    }
    return y;
}

double arcTan(double x) {
    return arcCos(1.0 / Sqrt(x * x + 1));
}

static double Exp(double x) {
    double t = 1.0;
    double y = 1.0;
    for (double k = 1.0; t > EPSILON; k += 1.0) {
        t *= x / k;
        y += t;
    }
    return y;
}

double Log(double x) {
    double z = 0.0;
    double y = 1.0;
    while (Abs(y - z) > EPSILON) {
        z = y;
        y = z + (x - Exp(z)) / Exp(z);
    }
    return y;
}
