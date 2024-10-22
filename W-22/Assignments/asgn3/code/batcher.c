#include "batcher.h"

static uint32_t bitwidth(uint32_t n) {
    uint32_t width = 1;
    while (n >>= 1) {
        width += 1;
    }
    return width;
}

// Translated from:
//  - math.mit.edu/~shor/18.310/batcher.pdf
//  - github.com/jgamble/Algorithm-Networksort/blob/master/lib/Algorithm/
//  - Knuth, Sorting and Searching, Algorithm 5.2.2M
void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t t = bitwidth(n);
    uint32_t p = 1 << (t - 1);

    while (p) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;

        while (d) {
            for (uint32_t i = 0; i + d < n; i += 1) {
                if ((i & p) == r && cmp(stats, A[i + d], A[i]) < 0) {
                    swap(stats, &A[i], &A[i + d]);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }

        p >>= 1;
    }
}
