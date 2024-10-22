#include "cocktail.h"
#include "util.h"
#include <inttypes.h>
#include <stdbool.h>

void cocktail_sort(uint32_t *arr, uint32_t length) {
  bool swapped = true;

  while (swapped && length > 1) {
    swapped = false;

    for (uint32_t i = 0; i < length - 2; i += 1) {
      if (compare(arr[i + 1], arr[i])) {
        swap(&arr[i], &arr[i + 1]);
        swapped = true;
      }
    }

    if (!swapped) {
      break;
    }

    swapped = false;

    for (uint32_t i = 0; i < length - 2; i += 1) {
      if (compare(arr[length - 1 - i], arr[length -2 - i])) {
        swap(&arr[length - 2 - i], &arr[length - 1 - i]);
        swapped = true;
      }
    }
  }

  return;
}
