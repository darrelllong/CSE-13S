// ☠️  2021 Darrell Long ☠️

#include "mathlib.h"

#include <assert.h>
#include <stdint.h>

static inline double abs(double x) { return x < 0 ? -x : x; }

static inline double signum(double x) { return x < 0 ? -1.0 : 1.0; }

// Find the integer part of x

static inline double truncate(double x) {
    return signum(x) * (uint64_t) abs(x);
}

// Floating point modulus: a % b, always non-negative

static inline double modulus(double a, double b) {
    double r = a - b * truncate(a / b);
    return a < 0 ? -abs(r) : abs(r);
}

// Square-root using the binary search algorithm

double biSqrt(double x) {
    double f = 1.0; // Scaling factor
    while (x > 1) { // Normalize to [0, 1) by reducing the exponent in IEEE 754
        x /= 4.0;   // Every 4 going in results in 2 going out since √4 = 2
        f *= 2.0;   // This should not disturb the mantissa
    }
    double m,                // mid-point
        l = 0.0,             // lower limit
        h = (x < 1) ? 1 : x; // upper limit
    do {
        m = (l + h) / 2.0;
        if (m * m < x) {
            l = m; // root is to the right
        } else {
            h = m; // root is to the left
        }
    } while (abs(l - h) > EPSILON);
    return f * m; // Rescale back to the original range
}

// Square-root using Newton's algorithm

double Sqrt(double x) {
    assert(x >= 0);
    double f = 1.0,     // Scaling factor
           y = x / 2.0; // Initial guess
    while (x > 1) { // Normalize to [0, 1) by reducing the exponent in IEEE 754
        x /= 4.0;   // Every 4 going in results in 2 going out since √4 = 2
        f *= 2.0;   // This should not disturb the mantissa
    }
    for (double guess = 0.0; abs(y - guess) > EPSILON; y = (y + x / y) / 2.0) {
        guess = y;
    }
    return f * y; // Rescale back to the original range
}

// Exp[x] =
//          2    3    4    5     6      7      8        9        10
//         x    x    x    x     x      x      x        x        x           11
// 1 + x + -- + -- + -- + --- + --- + ---- + ----- + ------ + ------- + O[x]
//         2    6    24   120   720   5040   40320   362880   3628800

double Exp(double x) {
    double term = 1, // Unity for multiplication that generates the terms
        sum = 1; // Zeroth term is always 1
    for (int k = 1; abs(term) > EPSILON; k += 1) {
        term *= x / k;
        sum += term;
    }
    return sum;
}

//                      x
// Newton iterate: -1 + -- + y
//                       y
//                      e

double Log(double x) {
    assert(x > 0);
    double y = 1.0; // Initial guess
    for (double p = Exp(y); abs(p - x) > EPSILON; p = Exp(y)) {
        y = y + x / p - 1;
    }
    return y;
}

//  y   y Log[x]
// x = e

double Pow(double x, double y) { return Exp(y * Log(x)); }

// Sin[x] =
//      3    5      7       9        11          13             15
//     x    x      x       x        x           x              x              16
// x - -- + --- - ---- + ------ - -------- + ---------- - ------------- + O[x]
//     6    120   5040   362880   39916800   6227020800   1307674368000

static inline double SinWork(double x, double epsilon) {
    x = modulus(x, 2 * M_PI); // Normalize to [-2π, 2π]
    double sgn = 1, val = x, trm = x;
    for (int k = 3; abs(trm) > epsilon; k += 2) {
        trm = trm * (x * x) / ((k - 1) * k);
        sgn = -sgn;
        val += sgn * trm;
    }
    return val;
}

// Cos[x] =
//      2    4    6      8        10         12           14
//     x    x    x      x        x          x            x             16
// 1 - -- + -- - --- + ----- - ------- + --------- - ----------- + O[x]
//     2    24   720   40320   3628800   479001600   87178291200

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

double Sin(double x) { return SinWork(x, EPSILON); }

double Cos(double x) { return CosWork(x, EPSILON); }

// Sec[x] = 1 / Cos[x]

double Sec(double x) { return 1 / Cos(x); }

// Csc[x] = 1 / Sin[x]

double Csc(double x) { return 1 / Sin(x); }

// Tan[x] = Sin[x] / Cos[x]

double Tan(double x) {
    double const epsilon = EPSILON; // Adjustable if needed
    return SinWork(x, epsilon) / CosWork(x, epsilon);
}

// Cot[x] = 1 / Tan[x]

double Cot(double x) { return 1 / Tan(x); }

// Newton iterate: x - Sec[x] (-a + Sin[x])
//
// When we get close to 1, we want to take a different approach and rely on
// trigometric identities.

double arcSin(double a) {
    assert(a >= -1 && a <= 1);
    double x = 0;
    if (abs(a) > 31.0 / 32.0) {
        return signum(a) * arcCos(Sqrt(1 - a * a));
    } else {
        while (abs(Sin(x) - a) > EPSILON) {
            x = x - (Sin(x) - a) / Cos(x);
        }
        return x;
    }
}

// Newton iterate: x + (-a + Cos[x]) Csc[x]
//
// When we get close to 1, we want to take a different approach and rely on
// trigometric identities.

double arcCos(double a) {
    assert(a >= -1 && a <= 1);
    double x = 1.0;
    if (abs(a) > 31.0 / 32.0) {
        return (a < 0) ? M_PI - arcSin(Sqrt(1 - a * a)) : arcSin(Sqrt(1 - a * a));
    } else {
        while (abs(Cos(x) - a) > EPSILON / 10) {
            x = x - (Cos(x) - a) / -Sin(x);
        }
        return x;
    }
}

// ArcCot[x] =
//             1
// ArcSin[------------], |x| < 1 and ArcTan[1/x], |x| ≥ 1
//                  2
//        Sqrt[1 + x ]
//
// When we exceed 1, we want to take a different approach and rely on trigometric identities.

double arcCot(double x) {
    if (abs(x) > 1) {
        return signum(x) * arcTan(1 / x);
    } else {
        return arcSin(1 / Sqrt(1 + x * x));
    }
}

// ArcTan[x] =
//             x
// ArcSin[------------], |x| < 1 and ArcCot[1/x], |x| ≥ 1
//                  2
//        Sqrt[1 + x ]
//
// When we exceed 1, we want to take a different approach and rely on trigometric identities.

double arcTan(double x) {
    if (abs(x) > 1) {
        return signum(x) * arcCot(1 / x);
    } else {
        return arcSin(x / Sqrt(1 + x * x));
    }
}
