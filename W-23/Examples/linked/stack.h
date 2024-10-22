#pragma once

#include "ll.h"

struct Stack {
    struct LinkedList ll;
};

struct Stack stack_new(void);

void stack_drop(struct Stack s);

size_t stack_len(struct Stack *s);

void stack_push(struct Stack *s, int data);

bool stack_pop(struct Stack *s, int *data);

void stack_print(struct Stack *s);
