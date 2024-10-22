#include "gaps.h"
#include "shell.h"
#include "util.h"

void shell_sort(uint32_t *A, uint32_t n) {
    for (uint32_t g = 0; g < GAPS; g += 1) {
        for (uint32_t i = gaps[g]; i < n; i += 1) {
            int64_t j = i;
            uint32_t temp = A[i];
            moves += 1;
            while (j >= gaps[g] && compare(temp, A[j - gaps[g]])) {
                A[j] = A[j - gaps[g]];
                j -= gaps[g];
                moves += 1;
            }
            A[j] = temp;
            moves += 1;
        }
    }
}
