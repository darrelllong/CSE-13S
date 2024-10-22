#include "stack.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    int64_t *items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->top = 0;
    s->capacity = capacity;
    s->items = (int64_t *)calloc(s->capacity, sizeof(int64_t));
    return s;
}

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
    }
    if (*s) {
        free(*s);
        *s = NULL;
    }
}

bool stack_empty(Stack *s) {
    return s->top == 0;
}

bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_push(Stack *s, int64_t x) {
    if (!stack_full(s)) {
        s->items[s->top++] = x;
        return true;
    }
    return false;
}

bool stack_pop(Stack *s, int64_t *x) {
    if (!stack_empty(s)) {
        *x = s->items[--s->top];
        return true;
    }
    return false;
}

void stack_print(Stack *s) {
    printf("[");
    for (uint32_t i = 0; i < s->top; i += 1) {
        printf("%" PRId64, s->items[i]);
        if (i + 1 != s->top) {
            printf(", ");
        }
    }
    printf("]\n");
}
