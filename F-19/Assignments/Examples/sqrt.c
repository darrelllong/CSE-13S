#include <math.h>
#include <stdio.h>

const long double epsilon = 1E-30;

long double Sqrt(long double x) {
    long double new = 1.0;
    long double old = 0.0;
    while (fabsl(new - old) > epsilon) {
        old = new;
        new = 0.5 * (new + x / new);
    }
    return new;
}

int main(void) {
    for (long double x = 1.0; x < 100.0; x += 0.01) {
        printf("sqrt(%4.2Lf) = %20.18Lf (𝛥 = % 20.19Lf)\n", x, Sqrt(x), sqrt(x)-Sqrt(x));
    }
}
