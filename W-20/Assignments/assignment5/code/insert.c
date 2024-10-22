#include "insert.h"
#include "util.h"
#include <inttypes.h>

extern uint64_t moves;

void insertion_sort(uint32_t *arr, uint32_t length) {
  for (uint32_t i = 1; i < length; i += 1) {
    uint32_t temp = arr[i];
    uint32_t j = i;
    moves += 1;

    while (j > 0 && compare(temp, arr[j - 1])) {
      arr[j] = arr[j - 1];
      j -= 1;
      moves += 1;
    }

    arr[j] = temp;
    moves += 1;
  }

  return;
}
