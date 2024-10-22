#include "stack.h"

struct Stack stack_new(void) {
    struct Stack s = {
        .ll = ll_new(),
    };
    return s;
}

void stack_drop(struct Stack s) {
    ll_drop(s.ll);
}

size_t stack_len(struct Stack *s) {
    return ll_len(&s->ll);
}

void stack_push(struct Stack *s, int data) {
    ll_add_back(&s->ll, data);
}

bool stack_pop(struct Stack *s, int *data) {
    return ll_pop_back(&s->ll, data);
}

void stack_print(struct Stack *s) {
    ll_print(&s->ll);
}
