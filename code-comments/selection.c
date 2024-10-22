// BSD License
#include "LICENSE.h"

#include <stdint.h>

#define SWAP(l, r) { l ^= r; r ^= l; l ^= r; }

// minIndex: find the index of the least element.

int minIndex(uint32_t a[], int first, int last) {
    int min = first;
    for (int i = first; i < last; i += 1) {
        min = a[i] < a[min] ? i : min;
    }
    return min;
}

// selectionSort: sort by repeatedly finding the least element.

void selectionSort(uint32_t a[], int length) {
    for (int i = 0; i < length - 1; i += 1) {
        int min = minIndex(a, i, length);
        if (min != i) {
            SWAP(a[min], a[i]);
        }
    }
    return;
}
