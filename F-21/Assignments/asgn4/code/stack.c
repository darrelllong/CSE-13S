#include "stack.h"
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->top = 0;
    s->capacity = capacity;
    s->items = (uint32_t *)calloc(capacity, sizeof(uint32_t));
    return s;
}

void stack_delete(Stack **s) {
    free((*s)->items);
    free(*s);
    *s = NULL;
}

bool stack_empty(Stack *s) {
    return s->top == 0;
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

bool stack_push(Stack *s, uint32_t item) {
    if (!stack_full(s)) {
        s->items[s->top++] = item;
        return true;
    }
    return false;
}

bool stack_pop(Stack *s, uint32_t *item) {
    if (!stack_empty(s)) {
        *item = s->items[--s->top];
        return true;
    }
    return false;
}

bool stack_peek(Stack *s, uint32_t *item) {
    if (!stack_empty(s)) {
        *item = s->items[s->top - 1];
        return true;
    }
    return false;
}

void stack_copy(Stack *dst, Stack *src) {
    if (dst->capacity == src->capacity) {
        for (uint32_t i = 0; i < src->top; i += 1) {
            dst->items[i] = src->items[i];
        }
        dst->top = src->top;
    }
}

void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}
