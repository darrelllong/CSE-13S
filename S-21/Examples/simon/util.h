#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

extern bool simple;
extern bool verbose;

void output(char *fmt, ...);

void check(bool cond, char *fmt, ...);

void words_to_bytes(uint64_t words[], uint8_t bytes[], uint8_t num_words);

void bytes_to_words(uint8_t bytes[], uint64_t words[], uint8_t num_bytes);

void print_hexstring(uint8_t bytes[], uint8_t num_bytes);

#endif
