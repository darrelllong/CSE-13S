#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (s) {
        s->top      = 0;
        s->capacity = capacity;
        s->items    = (Node **)calloc(s->capacity, sizeof(Node *));
    }
    return s;
}

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        for (uint32_t i = 0; i < (*s)->top; i += 1) {
            node_delete(&(*s)->items[i]);
        }
        free((*s)->items);
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

bool stack_push(Stack *s, Node *n) {
    if (!stack_full(s)) {
        s->items[s->top++] = n;
        return true;
    }
    return false;
}

bool stack_pop(Stack *s, Node **n) {
    if (!stack_empty(s)) {
        *n = s->items[--s->top];
        return true;
    }
    return false;
}

void stack_print(Stack *s) {
    printf("[");
    for (uint32_t i = 0; i < s->top; i += 1) {
        node_print(s->items[i]);
        if (i + 1 != s->top) {
            printf(", ");
        }
    }
    printf("]\n");
}
