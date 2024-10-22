#include <math.h>
#include <stdio.h>

double Exp(double x) {
  double term = 1.0, sum = term;
  for (double k = 1.0; term > 10e-12; k += 1.0) {
    term *= x / k;
    sum += term;
  }
  return sum;
}

double e() { return Exp(1.0); }

int main() {
  printf("e = %lf (𝚫 = %lf)\n", e(), e() - exp(1));
  return 0;
}
