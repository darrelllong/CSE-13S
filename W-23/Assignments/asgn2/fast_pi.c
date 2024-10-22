#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950

long double 𝛑(unsigned int n) {
    long double sum = 0.0, sixteen = 1.0;
    for (long double k = 0.0; k <= (long double) n; k += 1.0) {
	sum += ((long double) 4.0 / (8.0 * k + 1.0)
              - (long double) 2.0 / (8.0 * k + 4.0)
	      - (long double) 1.0 / (8.0 * k + 5.0)
	      - (long double) 1.0 / (8.0 * k + 6.0)) * sixteen;
	sixteen /= 16.0;
    }
    return sum;
}

int main(void) {
    long double old = 0.0;
    printf("sizeof(long double) = %lu\n", sizeof(long double));
    // for (unsigned int i = 0; i < 20; i += 1) {
	// long double pi = 𝛑(i);
	// printf("𝛑(%2u) = %41.40Lf (%31.31Lf)\n", i, pi, fabsl(PI - pi));
    // printf("∂ = %41.40Lf\n", old - pi); old = pi;
    // }
    return 0;
}
