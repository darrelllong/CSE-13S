#include <math.h>
#include <stdio.h>

const long double epsilon = 1e-15;

static int steps;

static double d;

long double Sqrt(long double x) {
  long double m, l = 0.0, h = (x < 1) ? 1 : x;
  steps = 0;
  do {
    steps += 1;
    m = (l + h) / 2.0;
    if (m * m < x) {
      l = m;
    } else {
      h = m;
    }
  } while (fabsl(l - h) > d);
  return m;
}

int main(void) {
    for (d = 0.1; d > epsilon; d /= 10.0)
    printf("Sqrt(%f) = %11.10Lf (𝛅 = %16.15Lf, steps = %d)\n", 2.0, Sqrt(2.0),
           fabsl(sqrtl(2.0) - Sqrt(2.0)), steps);
  return 0;
}
