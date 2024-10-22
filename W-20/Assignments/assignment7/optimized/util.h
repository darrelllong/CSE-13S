#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAGIC         0x8badbeef

#define ALPHABET      256

#define STOP_CODE     0
#define EMPTY_CODE    1
#define START_CODE    2
#define MAX_CODE      UINT16_MAX

#define BYTE          8
#define SHORT         16
#define BLOCK         4096

uint64_t bytes(uint64_t bits);

uint8_t bitwidth(uint16_t n);

void check(bool cond, char *fmt, ...);

#endif
