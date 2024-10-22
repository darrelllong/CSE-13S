#include "shell.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

static inline uint64_t power(uint64_t a, uint64_t d) {
    uint64_t v = 1;
    for (uint64_t p = a; d > 0; d >>= 1) {
        v *= (d & 0x1) ? p : 1;
        p *= p;
    }
    return v;
}

static inline uint32_t max_gap(uint32_t n) {
    return log(3 + 2 * n) / log(3);
}

static inline uint32_t knuth_gap(uint32_t i) {
    return (power(3, i) - 1) / 2;
}

void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    for (int64_t k = max_gap(n); k > 0; k -= 1) {
        uint32_t gap = knuth_gap(k);
        for (uint32_t i = gap; i < n; i += 1) {
            int64_t j = i;
            uint32_t temp = move(stats, A[i]);
            while (j >= gap && cmp(stats, temp, A[j - gap]) < 0) {
                A[j] = move(stats, A[j - gap]);
                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    }
}
