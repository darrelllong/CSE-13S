#include "binary.h"
#include "util.h"
#include <inttypes.h>

void binary_insertion_sort(uint32_t *arr, uint32_t n) {
  for (uint32_t i = 1; i < n; i += 1) {
    uint32_t temp = arr[i];
    uint32_t left = 0;
    uint32_t right = i;

    while (left < right) {
      uint32_t mid = left + ((right - left) / 2);

      if (compare(arr[mid], temp)) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }

    for (int64_t j = i; j > left; j -= 1) {
      swap(&arr[j - 1], &arr[j]);
    }
  }

  return;
}
