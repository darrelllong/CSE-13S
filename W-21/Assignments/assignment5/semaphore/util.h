#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

extern bool timed;
extern uint64_t moves;
extern uint64_t compares;

void swap(uint32_t *x, uint32_t *y);

bool compare(uint32_t x, uint32_t y);

void start_timer(void);

void stop_timer(void);

void print_stats(uint32_t elements);

void print_array(uint32_t *arr, uint32_t elements);

uint32_t *filled_array(uint32_t length, uint32_t seed);

void check(bool cond, const char *format, ...);

#endif
