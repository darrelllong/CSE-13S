#ifndef _MINSORT_H
#define _MINSORT_H

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

void minSort(uint32_t[], int);
#endif
