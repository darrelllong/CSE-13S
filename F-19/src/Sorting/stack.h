#ifndef _STACK_H
#define _STACK_H
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t item;

typedef struct stack {
  uint32_t size;
  uint32_t top;
  item *entries;
} stack;

#define MIN_STACK 128
#define INVALID 0xDeadD00d

stack *newStack();

void delStack(stack *);

item pop(stack *);

void push(stack *, item);

bool empty(stack *);

#endif
