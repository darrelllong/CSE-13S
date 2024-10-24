#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>
#include <stdbool.h>

#define MIN_CAPACITY 16

typedef struct Stack Stack;

Stack *stack_create(void);

void stack_delete(Stack **s);

uint32_t stack_size(Stack *s);

bool stack_empty(Stack *s);

bool stack_push(Stack *s, int64_t x);

bool stack_pop(Stack *s, int64_t *x);

void stack_print(Stack *s);

#endif
