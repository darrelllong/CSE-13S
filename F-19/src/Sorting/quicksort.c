#include "quicksort.h"
#include "stack.h"
#include <stdint.h>

int partition(uint32_t a[], int32_t low, int32_t high) {
  uint32_t pivotValue = a[(low + high) / 2];

  int32_t i = low - 1;
  int32_t j = high + 1;
  do {
    do {
      i += 1;
    } while (++compares && a[i] < pivotValue);
    do {
      j -= 1;
    } while (++compares && a[j] > pivotValue);
    if (i < j) {
      SWAP(a[i], a[j]);
    }
  } while (i < j);
  return j;
}

void quickSortI(uint32_t a[], int left, int right) {
  stack *s = newStack();

  push(s, left);
  push(s, right);
  while (!empty(s)) {
    int high = pop(s);
    int low = pop(s);
    int p = partition(a, low, high);
    if (p + 1 < high) {
      push(s, p + 1);
      push(s, high);
    }
    if (low < p) {
      push(s, low);
      push(s, p);
    }
  }
  delStack(s);
  return;
}

void quickSort(uint32_t a[], int32_t low, int32_t high) {
  if (low < high) {
    uint32_t p = partition(a, low, high);

    quickSort(a, low, p);
    quickSort(a, p + 1, high);
  }
  return;
}

void qSort(uint32_t a[], int length) {
  quickSort(a, 0, length - 1);
}

void qSortI(uint32_t a[], int length) {
  quickSortI(a, 0, length - 1);
}
