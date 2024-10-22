#include <math.h>
#include <stdio.h>

#define EPSILON 1e-14

double Abs(double x) { return x < 0 ? -x : x; }

double Cos(double x) {
    double s, v, t;
    s = v = t = 1.0;
    for (double k = 1.0; Abs(t) > EPSILON; k += 2.0) {
        t = t * (x * x) / ((k + 1) * k);
        s = -s;
        v += s * t;
    }
    return v;
}

int main(void) {
  for (double x = -2.0 * M_PI; x < 2 * M_PI; x += 0.1) {
    printf("cos(%3.2lf) = %+lf (%+20.19lf)\n", x, Cos(x), (cos(x) - Cos(x)));
  }
  return 0;
}
