#include "util.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//
// Calculates number of bytes needed to represent a certain number of bits.
//
// bits:      Number of bits.
// returns:   Number of bytes needed.
//
uint64_t bytes(uint64_t bits) {
  return ((bits % 8) || (!bits)) ? bits / 8 + 1 : bits / 8;
}

//
// Calculates the least number of bits needed to represent a uint16_t.
//
// n:       Number to calculate bitwidth of.
// returns: The calculated bitwidth.
//
uint8_t bitwidth(uint16_t n) {
  uint8_t msb_ind = 0;

  while (n > 1) {
    msb_ind += 1;
    n >>= 1;
  }

  return msb_ind + 1;
}

//
// Checks if a certain condition is satisfied.
// If not satisfied, the supplied format string is printed to stderr.
//
// cond:    Condition that should be true.
// fmt:     Format string to print if condition isn't met.
// ...:     Variable arguments for format string.
// returns: Void.
//
void check(bool cond, char *fmt, ...) {
  if (!cond) {
    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
    exit(EXIT_FAILURE);
  }

  return;
}
