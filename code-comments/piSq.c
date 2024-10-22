// BSD License
#include "LICENSE.h"

#include <math.h>
#include <stdio.h>

const long double epsilon = 1e-14;

static inline long double abs(long double x) {
    return (x < 0) ? -x : x;
}

long double Sqrt(long double x) {
    long double m, l = 0.0, h = (x < 1) ? 1 : x;
    do {
        m = (l + h) / 2.0;
        if (m * m < x) {
            l = m;
        } else {
            h = m;
        }
    } while (abs(l - h) > epsilon);
    return m;
}

int main(void) {
    printf("n = ");
    int n;
    if (scanf("%d", &n) < 1 || n < 1) {
        fprintf(stderr, "Oops!\n");
        return 1;
    }
    long double p = 0.0;
    for (long double x = 1.0; x < n; x += 1) {
        p += 1 / (x * x);
    }
    printf("%22.20Lf\t%22.20Lf\t%22.20Lf\n", Sqrt(6 * p), (long double) M_PI, Sqrt(6 * p) - M_PI);

    return 0;
}
