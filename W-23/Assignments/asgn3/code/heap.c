#include "heap.h"
#include <stdbool.h>

static uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first,
                          uint32_t last) {
  uint32_t left = first * 2;
  uint32_t right = left + 1;
  uint32_t child = left;

  if (right <= last && cmp(stats, A[left - 1], A[right - 1]) < 0) {
    child = right;
  }

  return child;
}

static void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
  bool found = false;
  uint32_t mother = first;
  uint32_t great = max_child(stats, A, mother, last);

  while (mother <= last / 2 && !found) {
    if (cmp(stats, A[mother - 1], A[great - 1]) < 0) {
      swap(stats, &A[mother - 1], &A[great - 1]);
      mother = great;
      great = max_child(stats, A, mother, last);
    } else {
      found = true;
    }
  }
}

static void build_heap(Stats *stats, uint32_t *A, uint32_t first,
                       uint32_t last) {
  for (int64_t mother = last / 2; mother >= first; mother -= 1) {
    fix_heap(stats, A, mother, last);
  }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
  uint32_t first = 1;
  uint32_t last = n;
  build_heap(stats, A, first, last);
  for (uint32_t leaf = last; leaf > first; leaf -= 1) {
    swap(stats, &A[first - 1], &A[leaf - 1]);
    fix_heap(stats, A, first, leaf - 1);
  }
}
