#include "stack.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

stack *newStack() {
  stack *s = (stack *)calloc(MIN_STACK, sizeof(stack));
  if (s) {
    s->size = MIN_STACK;
    s->top = 0;
    s->entries = (item *)calloc(MIN_STACK, sizeof(item));
    return s;
  } else {
    return NULL;
  }
}

void delStack(stack *s) {
  free(s->entries);
  free(s);
  return;
}

item pop(stack *s) {
  if (s->top > 0) {
    s->top -= 1;
    return s->entries[s->top];
  } else {
    return INVALID;
  }
}

void push(stack *s, item i) {
  if (s->top == s->size) {
    s->size *= 2;
    s->entries = (item *)realloc(s->entries, s->size * sizeof(item));
  }
  s->entries[s->top] = i;
  s->top += 1;
  return;
}

bool empty(stack *s) {
  return s->top == 0;
}