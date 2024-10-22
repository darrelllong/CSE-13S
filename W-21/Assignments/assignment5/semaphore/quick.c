#include "quick.h"
#include "util.h"
#include <inttypes.h>

void quick_sort(uint32_t *arr, int64_t n) {
  if (n < 2) {
    return;
  }

  int64_t i = 0;
  int64_t j = n - 1;
  uint32_t pivot = arr[n / 2];

  for (;;) {
    while (compare(arr[i], pivot)) {
      i += 1;
    }

    while (compare(pivot, arr[j])) {
      j -= 1;
    }

    if (i >= j) {
      break;
    }

    swap(&arr[i], &arr[j]);
    i += 1;
    j -= 1;
  }

  quick_sort(arr, i);
  quick_sort(arr + i, n - i);
  return;
}
