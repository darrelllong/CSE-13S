#include "queue.h"
#include "quick.h"
#include "stack.h"
#include "util.h"

uint32_t max_stack_size = 0;
uint32_t max_queue_size = 0;

static uint32_t max(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}

static int64_t partition(uint32_t *A, int64_t lo, int64_t hi) {
    uint32_t pivot = A[lo + ((hi - lo) / 2)];
    int64_t i = lo - 1, j = hi + 1;

    while (i < j) {
        i += 1;
        while (compare(A[i], pivot)) {
            i += 1;
        }
        j -= 1;
        while (compare(pivot, A[j])) {
            j -= 1;
        }
        if (i < j) {
            swap(&A[i], &A[j]);
        }
    }

    return j;
}

void quick_sort_stack(uint32_t *A, uint32_t n) {
    int64_t lo = 0, hi = n - 1;

    Stack *s = stack_create(n);
    stack_push(s, lo);
    stack_push(s, hi);

    while (!stack_empty(s)) {
        max_stack_size = max(max_stack_size, stack_size(s));
        stack_pop(s, &hi);
        stack_pop(s, &lo);
        int64_t p = partition(A, lo, hi);
        if (lo < p) {
            stack_push(s, lo);
            stack_push(s, p);
        }
        if (hi > p + 1) {
            stack_push(s, p + 1);
            stack_push(s, hi);
        }
    }

    stack_delete(&s);
}

void quick_sort_queue(uint32_t *A, uint32_t n) {
    int64_t lo = 0, hi = n - 1;

    Queue *q = queue_create(n);
    enqueue(q, lo);
    enqueue(q, hi);

    while (!queue_empty(q)) {
        max_queue_size = max(max_queue_size, queue_size(q));
        dequeue(q, &lo);
        dequeue(q, &hi);
        int64_t p = partition(A, lo, hi);
        if (lo < p) {
            enqueue(q, lo);
            enqueue(q, p);
        }
        if (hi > p + 1) {
            enqueue(q, p + 1);
            enqueue(q, hi);
        }
    }

    queue_delete(&q);
}
