#include "quick.h"

static uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1;

    for (uint32_t j = lo; j < hi; j += 1) {
        if (cmp(stats, A[j - 1], A[hi - 1]) < 0) {
            i += 1;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }

    swap(stats, &A[i], &A[hi - 1]);
    return i + 1;
}

static void sort(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, A, lo, hi);
        sort(stats, A, lo, p - 1);
        sort(stats, A, p + 1, hi);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    sort(stats, A, 1, n);
}
