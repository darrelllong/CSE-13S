#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

uint64_t bytes(uint64_t bits);

uint8_t bitwidth(uint16_t n);

void check(bool cond, char *fmt, ...);

#endif
