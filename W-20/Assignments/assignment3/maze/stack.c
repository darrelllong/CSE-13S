#include "stack.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_STACK 16

stack *newStack() {
  stack *s = (stack *) calloc(MIN_STACK, sizeof(stack));
  if (s) {
    s->size = MIN_STACK;
    s->top = 0;
    s->entries = (item *) calloc(MIN_STACK, sizeof(item));
    if (s->entries) {
      return s;
    }
  }
  free(s);
  return (void *)0;
}

bool pop(stack *s, item *i) {
  if (s->top > 0) {
    s->top -= 1;
    *i = s->entries[s->top];
    return true;
  } else {
    return false;
  }
}

bool push(stack *s, item *i) {
  if (s->top == s->size) {
    s->size *= 2;
    s->entries = (item *) realloc(s->entries, s->size * sizeof(item));
  }
  if (s && s->entries) {
    s->entries[s->top] = *i;
    s->top += 1;
    return true;
  }
  return false;
}

static char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void printStack(stack *s) {
  if (!emptyS(s)) {
    printf("%c", alpha[s->entries[0]]);
    for (uint32_t i = 1; i < s->top; i += 1) {
      printf(" -> %c", alpha[s->entries[i]]);
    }
    printf("\n");
  }
  return;
}
