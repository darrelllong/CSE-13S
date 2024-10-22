#include "exp.h"
#include <math.h>

// Written by DDEL

const double epsilon = 1E-15;

// Simple Taylor series expansion

double Exp(double x) {
  double term = 1.0;
  double sum = term;

  for (double k = 1.0; fabs(term) > epsilon; k += 1.0) {
    term = x / k * term;
    sum += term;
  }
  return sum;
}
