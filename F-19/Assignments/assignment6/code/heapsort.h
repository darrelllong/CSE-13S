#ifndef _HEAP_H
#define _HEAP_H

#ifndef SWAP
#define SWAP(a, b)                                                             \
  {                                                                            \
    uint32_t t = a;                                                            \
    a = b;                                                                     \
    b = t;                                                                     \
    moves += 3;                                                                \
  }
#endif

extern uint64_t moves, compares;

void heapSort(uint32_t *, uint32_t);
#endif
