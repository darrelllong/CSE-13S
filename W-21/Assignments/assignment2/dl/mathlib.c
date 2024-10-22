#include "mathlib.h"

#include <assert.h>
#include <stdint.h>

static inline double Abs(double x) {
    return x < 0 ? -x : x;
}

static inline double truncate(double x) {
    double sign = x < 0 ? -1.0 : 1.0;
    return sign * (uint64_t) Abs(x);
}

static inline double modulus(double a, double b) {
    double r = a - b * truncate(a / b);
    return a < 0 ? -Abs(r) : Abs(r);
}

double Sqrt(double x) {
    double y = 1.0;
	assert(x >= 0);
    for (double guess = 0.0; Abs(y - guess) > EPSILON; y = (y + x / y) / 2.0) {
        guess = y;
    }
    return y;
}

double Exp(double x) {
    double term = 1, sum = 1;
    for (int k = 1; Abs(term) > EPSILON; k += 1) {
        term *= x / k;
        sum += term;
    }
    return sum;
}

double Log(double x) {
    double y = 1.0;
	assert(x > 0);
    for (double p = Exp(y); Abs(p - x) > EPSILON; p = Exp(y)) {
        y = y + x / p - 1;
    }
    return y;
}

double Pow(double x, double y) {
    return Exp(y * Log(x));
}

static inline double SinWork(double x, double epsilon) {
    x = modulus(x, 2 * M_PI); // Normalize to [-2π, 2π]
    double sgn = 1, val = x, trm = x;
    for (int k = 3; Abs(trm) > epsilon; k += 2) {
        trm = trm * (x * x) / ((k - 1) * k);
        sgn = -sgn;
        val += sgn * trm;
    }
    return val;
}

static inline double CosWork(double x, double epsilon) {
    x = modulus(x, 2 * M_PI); // Normalize to [-2π, 2π]
    double sgn = 1, val = 1, trm = 1;
    for (int k = 1; trm > epsilon; k += 2) {
        trm = trm * (x * x) / ((k + 1) * k);
        sgn = -sgn;
        val += sgn * trm;
    }
    return val;
}

double Sin(double x) {
    return SinWork(x, EPSILON);
}

double Cos(double x) {
    return CosWork(x, EPSILON);
}

double arcSin(double a) {
    double x = 0;
    while (Abs(Sin(x) - a) > EPSILON) {
        x = x - (Sin(x) - a) / Cos(x);
    }
    return x;
}

double arcCos(double a) {
    double x = 1.0;
    while (Abs(Cos(x) - a) > EPSILON) {
        x = x - (Cos(x) - a) / -Sin(x);
    }
    return x;
}

double Tan(double x) {
    double const epsilon = EPSILON; // Adjustable if needed
    return SinWork(x, epsilon) / CosWork(x, epsilon);
}

double arcTan(double x) {
    return arcSin(x / Sqrt(1 + x * x));
}
