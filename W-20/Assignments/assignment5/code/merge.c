#include "merge.h"
#include "util.h"
#include <inttypes.h>
#include <stdlib.h>

extern uint64_t moves;

static void merge(uint32_t *arr, uint32_t l, uint32_t m, uint32_t r) {
  uint32_t half_len_l = m - l + 1;
  uint32_t half_len_r = r - m;

  uint32_t *left = (uint32_t *)calloc(half_len_l + 1, sizeof(uint32_t));
  uint32_t *right = (uint32_t *)calloc(half_len_r + 1, sizeof(uint32_t));
  check(left, "Failed to allocate left sub-array!\n");
  check(left, "Failed to allocate right sub-array!\n");

  for (uint32_t i = 0; i < half_len_l; i += 1) {
    left[i] = arr[l + i];
    moves += 1;
  }

  for (uint32_t i = 0; i < half_len_r; i += 1) {
    right[i] = arr[m + i + 1];
    moves += 1;
  }

  uint32_t i = 0, j = 0, k = l;

  while (i < half_len_l && j < half_len_r) {
    if (compare(left[i], right[j])) {
      arr[k++] = left[i++];
    } else {
      arr[k++] = right[j++];
    }

    moves += 1;
  }

  while (i < half_len_l) {
    arr[k++] = left[i++];
    moves += 1;
  }

  while (j < half_len_r) {
    arr[k++] = right[j++];
    moves += 1;
  }

  free(left);
  left = NULL;
  free(right);
  right = NULL;
  return;
}

void merge_sort(uint32_t *arr, uint32_t l, uint32_t r) {
  if (l < r) {
    uint32_t m = l + (r - l) / 2;
    merge_sort(arr, l, m);
    merge_sort(arr, m + 1, r);
    merge(arr, l, m, r);
  }

  return;
}
