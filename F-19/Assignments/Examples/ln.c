#include <math.h>
#include <stdio.h>

#define EPSILON 1.0e-16

long double Exp(long double x) {
  long double term = 1.0; // Zero'th order term
  long double sum  = term;
  for (long double k = 1.0; fabsl(term) > EPSILON; k += 1.0)
  {
    term = x / k * term; // Compute each term in the Taylor series
    sum += term; // Sum them up
  }
  return sum;
}

// Computer the logarithm by inverting the exponential function using Newton's
// method. This should provide quadratic convergence.
//
// f(x) = x - exp(x)
// f'(x) = exp(x)

long double doLog(long double x) {
  long double y = 1.0;
  long double p = Exp(y);
  while (fabsl(p - x) > EPSILON) {
    y = y + (x - p) / p;
    p = Exp(y);
  }
  return y;
}

// A little algebra should make it converge faster: log(x) = -log(1/x)

long double Log(long double t) {
  if (t > 2.0) {
    return -doLog(1.0 / t);
  } else {
    return doLog(t);
  }
}

int main(void) {
  for (long double r = 1; r < 1000000000; r *= 10) {
    printf("Log(%Lf) = %Lf, log(%Lf) = %lf\n", r, Log(r), r, log(r));
  }
}
