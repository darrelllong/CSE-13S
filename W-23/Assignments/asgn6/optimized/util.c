#include "util.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define IS_POW_2(x)   (!(x & (x - 1)))

uint64_t bytes(uint64_t bits) {
  return ((bits % 8) || (!bits)) ? bits / 8 + 1 : bits / 8;
}

uint8_t bitwidth(uint16_t code) {
  static uint8_t width = 2;

  if (code == START_CODE) {
    width = 2;
  } else if (IS_POW_2(code)) {
    width += 1;
  }

  return width;
}

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
