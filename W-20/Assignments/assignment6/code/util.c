#include "util.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t max(uint64_t x, uint64_t y) {
  return (x > y) ? x : y;
}

void str_lowercase(char *str) {
  for (uint64_t i = 0; i < strlen(str); i += 1) {
    str[i] = tolower(str[i]);
  }

  return;
}

void check(bool cond, const char *format, ...) {
  if (!cond) {
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
    exit(1);
  }

  return;
}
