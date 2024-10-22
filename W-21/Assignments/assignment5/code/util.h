#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

extern uint64_t moves;
extern uint64_t compares;

void swap(uint32_t *x, uint32_t *y);

bool compare(uint32_t x, uint32_t y);

void fill_array(uint32_t *A, uint32_t n, uint32_t seed);

void reset_stats(void);

void print_stats(uint32_t n);

void print_array(uint32_t *A, uint32_t n);

void check(bool cond, const char *fmt, ...);

#endif
