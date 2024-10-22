#include "select.h"
#include "util.h"
#include <inttypes.h>

extern uint64_t compares;

static uint32_t min_index(uint32_t *a, uint32_t start, uint32_t end) {
  uint32_t min_ind = start;

  for (uint32_t i = start; i < end; i += 1) {
    if (compare(a[i], a[min_ind])) {
      min_ind = i;
    }
  }

  return min_ind;
}

void selection_sort(uint32_t *a, uint32_t length) {
  for (uint32_t i = 0; i < length - 1; i += 1) {
    uint32_t min_ind = min_index(a, i, length);
    if (i != min_ind) {
      swap(&a[i], &a[min_ind]);
    }
  }

  return;
}
