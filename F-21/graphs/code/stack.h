#ifndef __STACK_H__
#define __STACK_H__

#include <stdbool.h>
#include <stdint.h>

#define Item uint32_t

typedef struct Stack Stack;

Stack *stack_create(uint32_t capacity);

void stack_delete(Stack **s);

bool stack_empty(Stack *s);

bool stack_full(Stack *s);

uint32_t stack_size(Stack *s);

bool stack_push(Stack *s, Item x);

bool stack_pop(Stack *s, Item *x);

void stack_print(Stack *s);

#endif
