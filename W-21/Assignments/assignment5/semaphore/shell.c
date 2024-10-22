#include "shell.h"
#include "util.h"
#include <inttypes.h>
#include <stdbool.h>

static bool next_gap(uint32_t *gap) {
  if (*gap == 1) {
    return false;
  }

  *gap = (*gap <= 2) ? 1 : 5 * *gap / 11;
  return true;
}

void shell_sort(uint32_t *arr, uint32_t n) {
  uint32_t gap = n;

  while (next_gap(&gap)) {
    for (uint32_t i = 0; i < n; i += 1) {
      for (int64_t j = i; j >= gap; j -= gap) {
        if (compare(arr[j], arr[j - gap])) {
          swap(&arr[j], &arr[j - gap]);
        }
      }
    }
  }

  return;
}
