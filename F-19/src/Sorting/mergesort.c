#include "mergesort.h"
#include <stdint.h>
#include <stdlib.h>

void merge(uint32_t values[], uint32_t left[], uint32_t right[], int middle,
    int total) {
  int left_size = middle, right_size = total - middle;
  int i = 0, j = 0;

  // Loop through values array
  for (int k = 0; k < total; k += 1) {
    // If elements left on left and either no elements on right
    // or left is larger than right, then use next left value
    if (++compares && i < left_size && (j >= right_size || left[i] <= right[j])) {
      values[k] = left[i];
      i += 1;
      moves += 1;
    } else // else use next right value
    {
      values[k] = right[j];
      j += 1;
      moves += 1;
    }
  }
}

void mergeSort(uint32_t values[], int n) {
  // If 1 or less elements stop
  if (n > 1) {

    // Set middle value, and left and right arrays
    int middle = n / 2;
    uint32_t *left  = (uint32_t *) malloc(middle * sizeof(uint32_t));
    uint32_t *right = (uint32_t *) malloc((n - middle) * sizeof(uint32_t));

    for (int i = 0; i < middle; i += 1) // Left side
    {
      left[i] = values[i];
      moves += 1;
    }
    for (int i = 0; i < n - middle; i += 1) // Right side
    {
      right[i] = values[i + middle];
      moves += 1;
    }

    mergeSort(left, middle); // Sort left
    mergeSort(right, n - middle); // Sort right

    // Merge sorted arrays
    merge(values, left, right, middle, n);
    free(left);
    free(right);
  }
  return;
}
