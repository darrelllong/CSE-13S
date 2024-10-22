#include <math.h>
#include <stdio.h>

const long double epsilon = 1E-10;

long double Exp(long double x) {
  long double term = 1.0;
  long double sum = term;
  for (long double k = 1.0; fabsl(term) > epsilon; k += 1.0) {
    term = x / k * term;
    sum += term;
  }
  return sum;
}

int main(void) {
  for (long double x = 0; x < 10.0; x += 0.1) {
      printf("% 6.4Lf\t% 10.8Lf\t% 10.8Lf\t% 12.10Lf\n", x, Exp(x), expl(x),
Exp(x)-expl(x));

  }
  return 0;
}
