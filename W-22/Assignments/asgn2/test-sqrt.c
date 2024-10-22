#include <math.h>
#include <stdio.h>

#define EPSILON 1e-14

static inline double Abs(double x) { return x < 0 ? -x : x; }

double Sqrt(double y) {
  double x_n = 1.0;
  double old = 0.0;
  while (Abs(x_n - old) > EPSILON) {
    old = x_n;
printf("%lf\n", old);
    x_n = 0.5 * (x_n + y / x_n);
  }
  return x_n;
}

int main(void) {
  for (double x = 0.0; x < 10.0; x += 1.0) {
    printf("√ %3.2lf = %lf (% 20.19lf)\n", x, Sqrt(x), (sqrt(x) - Sqrt(x)));
  }
  return 0;
}
