#include "util.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int exit_status = EXIT_SUCCESS;

uint64_t max(uint64_t x, uint64_t y) {
  return (x > y) ? x : y;
}

void str_lowercase(char *str) {
  for (uint64_t i = 0; i < strlen(str); i += 1) {
    str[i] = tolower(str[i]);
  }

  return;
}

void errprint(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stderr, format, arg);
  va_end(arg);
  exit_status = EXIT_FAILURE;
  return;
}

void assert_ptr(void *ptr, const char *format, ...) {
  if (!ptr) {
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
    exit(EXIT_FAILURE);
  }

  return;
}

void assert_stmt(bool stmt, const char *format, ...) {
  if (!stmt) {
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
    exit(EXIT_FAILURE);
  }

  return;
}
