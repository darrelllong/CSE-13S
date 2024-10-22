#include "stack.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};

Stack *stack_create(void) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->top = 0;
    s->capacity = MIN_CAPACITY;
    s->items = (uint32_t *) calloc(s->capacity, sizeof(uint32_t));
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

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_empty(Stack *s) {
    return s->top == 0;
}

bool stack_push(Stack *s, int64_t x) {
    if (s->top == s->capacity) {
        s->capacity *= 2;
        s->items = (uint32_t *) realloc(s->items, s->capacity * sizeof(uint32_t));
    }
    s->items[s->top++] = x;
    return true;
}

bool stack_pop(Stack *s, int64_t *x) {
    if (s->top == 0) {
        return false;
    }
    *x = s->items[--s->top];
    return true;
}

void stack_print(Stack *s) {
    printf("[");
    for (uint32_t i = 0; i < s->top; i += 1) {
        printf("%" PRIu32, s->items[i]);
        if (i + 1 != s->top) {
            printf(", ");
        }
    }
    printf("]\n");
}
