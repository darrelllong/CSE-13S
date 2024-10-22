#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

bool simple = false;
bool verbose = false;

void output(char *fmt, ...) {
  if (verbose) {
    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
  }

  return;
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

void words_to_bytes(uint64_t words[], uint8_t bytes[], uint8_t num_words) {
  uint8_t j = 0;

  for (uint8_t i = 0; i < num_words; i += 1) {
    bytes[j] = (uint8_t)words[i];
    bytes[j + 1] = (uint8_t)(words[i] >> 8);
    bytes[j + 2] = (uint8_t)(words[i] >> 16);
    bytes[j + 3] = (uint8_t)(words[i] >> 24);
    bytes[j + 4] = (uint8_t)(words[i] >> 32);
    bytes[j + 5] = (uint8_t)(words[i] >> 40);
    bytes[j + 6] = (uint8_t)(words[i] >> 48);
    bytes[j + 7] = (uint8_t)(words[i] >> 56);
    j += 8;
  }

  return;
}

void bytes_to_words(uint8_t bytes[], uint64_t words[], uint8_t num_bytes) {
  uint8_t j = 0;

  for (uint8_t i = 0; i < num_bytes / 8; i += 1) {
    words[i] = (uint64_t)bytes[j];
    words[i] |= ((uint64_t)bytes[j + 1] << 8);
    words[i] |= ((uint64_t)bytes[j + 2] << 16);
    words[i] |= ((uint64_t)bytes[j + 3] << 24);
    words[i] |= ((uint64_t)bytes[j + 4] << 32);
    words[i] |= ((uint64_t)bytes[j + 5] << 40);
    words[i] |= ((uint64_t)bytes[j + 6] << 48);
    words[i] |= ((uint64_t)bytes[j + 7] << 56);
    j += 8;
  }

  return;
}

void print_hexstring(uint8_t bytes[], uint8_t num_bytes) {
  for (uint8_t i = 0; i < num_bytes; i += 1) {
    if (i && !(i % 8)) {
      output(" ");
    }

    output("%02" PRIx8, bytes[i]);
  }

  output("\n");
  return;
}
