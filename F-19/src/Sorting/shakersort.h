#ifndef _SHAKERSORT_H
#define _SHAKERSORT_H
#include <stdint.h>

#ifndef SWAP
#define SWAP(x, y)                                                             \
  {                                                                            \
    uint32_t t = x;                                                            \
    x = y;                                                                     \
    y = t;                                                                     \
    moves += 3;                                                                \
  }
#endif

extern uint64_t moves, compares;

void shakerSort(uint32_t[], int);
#endif
