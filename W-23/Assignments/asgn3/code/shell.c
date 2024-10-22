#include "shell.h"

#include "gaps.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
  for (uint32_t const *gptr = gaps; gptr != gaps + GAPS; ++gptr) {
    uint32_t gap = *gptr;
    for (uint32_t i = gap; i < n; i += 1) {
      uint32_t j = i;
      uint32_t temp = move(stats, A[i]);
      while (j >= gap && cmp(stats, temp, A[j - gap]) < 0) {
        A[j] = move(stats, A[j - gap]);
        j -= gap;
      }
      A[j] = move(stats, temp);
    }
  }
}
