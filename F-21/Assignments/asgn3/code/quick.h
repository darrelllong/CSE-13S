#ifndef __QUICK_H__
#define __QUICK_H__

#include "stats.h"
#include <stdint.h>

void quick_sort(Stats *stats, uint32_t *A, uint32_t n);

void quick_sort_stack(Stats *stats, uint32_t *A, uint32_t n);

#endif
