#ifndef _QUICKSORT_H
#define _QUICKSORT_H

extern uint64_t moves, compares;

#ifndef SWAP
#define SWAP(x, y)                                                             \
  {                                                                            \
    uint32_t t = x;                                                            \
    x = y;                                                                     \
    y = t;                                                                     \
    moves += 3;                                                                \
  }
#endif

void qSort(uint32_t a[], int length);
void qSortI(uint32_t a[], int length);
#endif
