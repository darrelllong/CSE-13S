#include <math.h>
#include <stdio.h>

const long double epsilon = 1e-15;

static inline long double abs(double x) { return (x < 0) ? -x : x; }

static int steps;

long double Sqrt(long double x) {
  long double f = 1.0;
  while (x > 1) {
    x /= 4.0;
    f *= 2.0;
  }
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
  } while (abs(l - h) > epsilon);
  return f * m;
}

int main(void) {
  for (long double x = 0.01; x < 1000000000000000.0; x *= 10.0) {
    printf("Sqrt(%5.2Lf) = %11.10Lf (𝛅 = %11.10Lf, steps = %d)\n", x, Sqrt(x),
           abs(sqrtl(x) - Sqrt(x)), steps);
  }
  return 0;
}
