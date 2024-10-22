#ifndef _HEAP_H_
#define _HEAP_H_

// heap.h
//
// DL
// Mon Feb  1 18:49:55 PST 1993

#include <inttypes.h>
#include <stdbool.h>

struct item {
  char *host;
  uint32_t uniq;
  long time;
};

typedef struct item node;

struct head {
  node *base;
  uint32_t count, allocation;
};

typedef struct head heap;

extern bool empty(heap *);

extern node top(heap *), dequeue(heap *);

extern void enqueue(heap *, node);
#endif
