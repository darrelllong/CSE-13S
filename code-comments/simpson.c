// BSD License
#include "LICENSE.h"

#include <math.h>
#include <stdio.h>

double integrate(double (*f)(double), double a, double b, double n) {
    double h = (b - a) / n; // Interval width
    double sum = 0.0;       // Accumulator
    sum += f(a);            // First term
    double tmp = 0.0;       // Even terms
    for (int j = 1; j <= floor(n / 2 - 1); j += 1) {
        tmp += f(a + 2 * j * h);
    }
    sum += 2.0 * tmp;
    tmp = 0.0;              // Odd terms
    for (int j = 1; j <= floor(n / 2); j += 1) {
        tmp += f(a + (2 * j - 1) * h);
    }
    sum += 4.0 * tmp;
    sum += f(b);            // Last term
    sum *= h / 3.0;
    return sum;
}

double g(double x) { return exp(-x * x); }

int main(void) {
    double p = integrate(g, -10.0, 10.0, 100);
    double 𝛑 = p * p;
    printf("𝛑 = %22.20lf (𝚫 = %22.20lf)\n", 𝛑, M_PI - 𝛑);
    return 0;
}
