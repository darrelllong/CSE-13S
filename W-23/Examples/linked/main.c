#include "ll.h"
#include "stack.h"
#include "queue.h"
#include <assert.h>

void test_ll(void) {
    struct LinkedList ll = ll_new();

    // Add 4 items.
    ll_add_back(&ll, 1);
    ll_add_back(&ll, 2);
    ll_add_back(&ll, 3);
    ll_add_back(&ll, 4);
    assert(ll_len(&ll) == 4);
    ll_print(&ll);

    // Add another 4 items.
    ll_add_front(&ll, -1);
    ll_add_front(&ll, -2);
    ll_add_front(&ll, -3);
    ll_add_front(&ll, -4);
    assert(ll_len(&ll) == 8);
    ll_print(&ll);

    // Pop the front item.
    int front;
    assert(ll_pop_front(&ll, &front));
    assert(front == -4);
    assert(ll_len(&ll) == 7);
    ll_print(&ll);

    // Pop the back item.
    int back;
    assert(ll_pop_back(&ll, &back));
    assert(back == 4);
    assert(ll_len(&ll) == 6);
    ll_print(&ll);

    ll_drop(ll);
}

void test_stack(void) {
    struct Stack s = stack_new();

    // Add 4 items.
    stack_push(&s, 1);
    stack_push(&s, 2);
    stack_push(&s, 3);
    stack_push(&s, 4);
    assert(stack_len(&s) == 4);
    stack_print(&s);

    // Pop each item.
    for (int i = 4; i > 0; i--) {
        int top;
        assert(stack_pop(&s, &top));
        assert(top == i);
        stack_print(&s);
    }

    assert(stack_len(&s) == 0);
    stack_drop(s);
}

void test_queue(void) {
    struct Queue q = queue_new();

    // Add 4 items.
    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);
    enqueue(&q, 4);
    assert(queue_len(&q) == 4);
    queue_print(&q);

    // Pop each item.
    for (int i = 1; i <= 4; i++) {
        int front;
        assert(dequeue(&q, &front));
        assert(front == i);
        queue_print(&q);
    }

    assert(queue_len(&q) == 0);
    queue_drop(q);
}

int main(void) {
    test_ll();
    test_stack();
    test_queue();
    return 0;
}
