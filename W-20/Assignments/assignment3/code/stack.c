#include "stack.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MINIMUM 26

Stack *stack_create(void) {
  Stack *s = (Stack *) calloc(1, sizeof(Stack));
  if (!s) {
    perror("calloc");
    exit(-1);
  }

  s->top = 0;
  s->capacity = MINIMUM;

  s->items = (uint32_t *) malloc(s->capacity * sizeof(uint32_t));
  if (!s->items) {
    perror("malloc");
    exit(-1);
  }

  return s;
}

void stack_delete(Stack *s) {
  free(s->items);
  s->items = NULL;
  free(s);
  return;
}

bool stack_empty(Stack *s) {
  return s->top == 0;
}

uint32_t stack_size(Stack *s) {
  return s->top;
}

bool stack_push(Stack *s, uint32_t item) {
  if (s->top == s->capacity) {
    s->capacity *= 2;
    s->items = (uint32_t *) realloc(s->items, s->capacity * sizeof(uint32_t));

    if (!s->items) {
      perror("realloc");
      exit(-1);
    }
  }

  s->items[s->top++] = item;
  return true;
}

bool stack_pop(Stack *s, uint32_t *item) {
  if (stack_empty(s)) {
    return false;
  }

  *item = s->items[--s->top];
  return false;
}

void stack_print(Stack *s) {
  for (uint32_t i = 0; i < s->top; i += 1) {
    printf("%c", s->items[i] + 'A');
    if (i != s->top - 1) {
      printf(" -> ");
    }
  }

  printf("\n");
  return;
}
