#include "bubble.h"
#include "util.h"

void bubble_sort(uint32_t *A, uint32_t n) {
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (uint32_t i = 1; i < n; i += 1) {
            if (compare(A[i], A[i - 1])) {
                swap(&A[i - 1], &A[i]);
                swapped = true;
            }
        }
        n -= 1;
    }
}
