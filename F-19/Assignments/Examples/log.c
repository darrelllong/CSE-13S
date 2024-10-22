#include <math.h>
#include <stdio.h>

const long double epsilon = 1.0E-18;

long double Exp(long double x) {
  long double term = 1.0; // Zero'th order term
  long double sum = term;
  for (long double k = 1.0; fabsl(term) > epsilon; k += 1.0) {
    term = x / k * term; // Compute each term in the Taylor series
    sum += term; // Sum them up
  }
  return sum;
}

// Computer the logarithm by inverting the exponential function using Newton's
// method. This should provide cubic convergence.

long double doLog(long double x) {
  long double y = 1.0;
  long double p = Exp(y);
  while (fabsl(p - x) > epsilon) {
    y = y + 2.0 * (x - p) / (x + p);
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
  for (long double x = 1; x < 1000000000; x *= 10) {
    printf("log(%4.2Lf) = %20.18Lf (𝛥 = % 20.19Lf)\n", x, Log(x),
        Log(x) - logl(x));
  }
}
