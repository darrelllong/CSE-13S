#include <stdbool.h>
#include <stdint.h>
#include "bubblesort.h"

void bubbleSort(uint32_t a[], int length) {
  bool swapped;
  do {
    swapped = false;
    for (int i = 1; i < length; i += 1) {
      compares += 1;
      if (a[i - 1] > a[i]) {
        SWAP(a[i - 1], a[i]);
        swapped = true;
      }
    }
    length -= 1; // Last is the largest, so move the bottom up
  } while (swapped);
  return;
}
