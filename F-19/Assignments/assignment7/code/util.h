#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

extern int exit_status;

uint64_t max(uint64_t x, uint64_t y);

void str_lowercase(char *str);

void errprint(const char *format, ...);

void assert_ptr(void *ptr, const char *format, ...);

void assert_stmt(bool stmt, const char *format, ...);

#endif
