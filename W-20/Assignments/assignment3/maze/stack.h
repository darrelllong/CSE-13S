#ifndef _STACK_H
#define _STACK_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint32_t item;

typedef struct stack {
  uint32_t size;
  uint32_t top;
  item *entries;
} stack;

extern stack *newStack();

extern bool pop(stack *, item *);

extern bool push(stack *, item *);

static inline void delStack(stack *s) {
  free(s->entries);
  free(s);
  return;
}

static inline bool emptyS(stack *s) { return s->top == 0; }

void printStack(stack *);

#endif
