#include "quick.h"
#include "stack.h"
#include "util.h"

static int64_t partition(uint32_t *A, int64_t lo, int64_t hi) {
    uint32_t pivot = A[lo + ((hi - lo) / 2)];

    int64_t i = lo - 1;
    int64_t j = hi + 1;

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

static void quick_sorter(uint32_t *A, int64_t left, int64_t right) {
    Stack *s = stack_create();
    stack_push(s, left);
    stack_push(s, right);

    int64_t lo = 0;
    int64_t hi = 0;
    while (!stack_empty(s)) {
        stack_pop(s, &hi);
        stack_pop(s, &lo);
        int64_t p = partition(A, lo, hi);
        if (p + 1 < hi) {
            stack_push(s, p + 1);
            stack_push(s, hi);
        }
        if (lo < p) {
            stack_push(s, lo);
            stack_push(s, p);
        }
    }

    stack_delete(&s);
}

void quick_sort(uint32_t *A, uint32_t n) {
    quick_sorter(A, 0, n - 1);
}
