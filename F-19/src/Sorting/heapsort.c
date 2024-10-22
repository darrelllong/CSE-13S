#include "heapsort.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Adapted from Sara Baase's "Computer Algorithms".

int maxChild(uint32_t *seq, uint32_t first, uint32_t last) {
  uint32_t left = 2 * first, right = left + 1;
  if (++compares && right <= last && seq[right - 1] > seq[left - 1]) {
    return right;
  } else {
    return left;
  }
}

void fixHeap(uint32_t *seq, uint32_t first, uint32_t last) {
  uint32_t father = first, great;
  bool found = false;
  great = maxChild(seq, father, last);
  while (++compares && father <= last / 2 && !found) {
    if (++compares && seq[father - 1] < seq[great - 1]) {
      SWAP(seq[father - 1], seq[great - 1]);
      father = great;
      great = maxChild(seq, father, last);
    } else {
      found = true;
    }
  }
  return;
}

void buildHeap(uint32_t *seq, uint32_t first, uint32_t last) {
  for (uint32_t father = last / 2; father >= first; father -= 1) {
    fixHeap(seq, father, last);
  }
  return;
}

void sortHeap(uint32_t *seq, uint32_t first, uint32_t last) {
  buildHeap(seq, first, last);
  for (uint32_t leaf = last; leaf >= first + 1; leaf -= 1) {
    SWAP(seq[first - 1], seq[leaf - 1]);
    fixHeap(seq, first, leaf - 1);
  }
  return;
}

void heapSort(uint32_t *seq, uint32_t last) {
  sortHeap(seq, 1, last);
  return;
}
