#include "minsort.h"
#include <stdint.h>

// minIndex: find the index of the least element.

int minIndex(uint32_t a[], int first, int last) {
  int min = first;
  for (int i = first; i < last; i += 1) {
    min = a[i] < a[min] ? i : min;
    compares += 1;
  }
  return min;
}

// minSort: sort by repeatedly finding the least element.

void minSort(uint32_t a[], int length) {
  for (int i = 0; i < length - 1; i += 1) {
    int min = minIndex(a, i, length);
    if (min != i) {
      SWAP(a[min], a[i]);
    }
  }
  return;
}
