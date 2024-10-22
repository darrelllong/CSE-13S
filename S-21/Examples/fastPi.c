#include <stdio.h>
#include <math.h>

long double 𝛑(unsigned int n) {
    long double sum = 0.0, sixteen = 1.0;
    for (long double k = 0.0; k <= (long double) n; k += 1.0) {
	sum += (4.0 / (8.0 * k + 1.0)
              - 2.0 / (8.0 * k + 4.0)
	      - 1.0 / (8.0 * k + 5.0)
	      - 1.0 / (8.0 * k + 6.0)) * sixteen;
	sixteen /= 16.0;
    }
    return sum;
}

int main(void) {
    for (unsigned int i = 0; i < 20; i += 1) {
	long double pi = 𝛑(i);
	printf("𝛑(%2u) = %31.30Lf (%31.31Lg)\n", i, pi, fabsl(M_PI - pi));
    }
    return 0;
}
