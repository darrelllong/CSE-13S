#include <stdbool.h>
#include <stdint.h>
#include "bubblesort.h"

void shakerSort(uint32_t a[], int length) {
  bool swapped;
  int low = 0, high = length;
  do {
    swapped = false;
    for (int i = low + 1; i < high; i += 1) {
      compares += 1;
      if (a[i - 1] > a[i]) {
        SWAP(a[i - 1], a[i]);
        swapped = true;
      }
    }
    high -= 1; // Last is the largest, so move the bottom up
    for (int i = high; i > low; i -= 1) {
      compares += 1;
      if (a[i - 1] > a[i]) {
        SWAP(a[i - 1], a[i]);
        swapped = true;
      }
    }
    low += 1; // First is the smallest, so move the top down
  } while (swapped);
  return;
}
