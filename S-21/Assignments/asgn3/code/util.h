#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

extern uint64_t moves;
extern uint64_t compares;

typedef enum { BUBBLE, SHELL, QUICKSTACK, QUICKQUEUE, SORTS } Sorts;

void swap(uint32_t *x, uint32_t *y);

bool compare(uint32_t x, uint32_t y);

void fill_array(uint32_t *A, uint32_t n, uint32_t seed, bool ordered);

void reset_stats(void);

void print_stats(uint32_t n, Sorts s);

void print_array(uint32_t *A, uint32_t n);

void check(bool cond, const char *fmt, ...);

#endif
