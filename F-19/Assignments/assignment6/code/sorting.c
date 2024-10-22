#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bubblesort.h"
#include "bv.h"
#include "heapsort.h"
#include "insertionsort.h"
#include "mergesort.h"
#include "minsort.h"
#include "quicksort.h"
#include "shakersort.h"
#include "shellsort.h"
#include "stack.h"

#if OLD == true
#define RANDOM rand
#define SRANDOM srand
#else
#define RANDOM random
#define SRANDOM srandom
#endif

#ifndef MASK
#define MASK 0x00ffffff
#endif

enum sorts {
  MinSort,
  BubbleSort,
  ShakerSort,
  InsertionSort,
  ShellSort,
  QuickSort,
  QSI,
  MergeSort,
  HeapSort,
  EndSort, 
  Ordered
};

#define MAX 100
#define WIDTH 7

uint64_t moves, compares;

static int printMax = MAX;

void fillArray(uint32_t a[], int length, bool inOrder) {
  for (int i = 0; i < length; i += 1) {
    a[i] = inOrder ? i : RANDOM() & MASK;
  }
  return;
}

void printArray(uint32_t a[], int length) {
  for (int i = 0; i < length && i < printMax; i += 1) {
    printf("%10d", a[i]);
    if ((i + 1) % WIDTH == 0) {
      printf("\n");
    }
  }
  if (printMax % WIDTH != 0 || length % WIDTH != 0) {
    printf("\n");
  }
  return;
}

#define OPTIONS "-uoAmbzisqQMhp:r:n:"

int main(int argc, char **argv) {
  int c = 0;
  int seed = 8062022; // Default random seed
  int count = 100; // Sort 100 by default

  uint32_t *a; // Array to be sorted

  bitV *sortSet = newVec(EndSort); // Set of sorts to perform

  if (sortSet == NULL) { exit(1); }

  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'A': {
      for (enum sorts s = MinSort; s < EndSort; s += 1)
        setBit(sortSet, s);
      break;
    }
    case 'u': {
      setBit(sortSet, EndSort);
      break;
    }
    case 'o': {
      setBit(sortSet, Ordered);
      break;
    }
    case 'm': {
      setBit(sortSet, MinSort);
      break;
    }
    case 'b': {
      setBit(sortSet, BubbleSort);
      break;
    }
    case 'z': {
      setBit(sortSet, ShakerSort);
      break;
    }
    case 'i': {
      setBit(sortSet, InsertionSort);
      break;
    }
    case 's': {
      setBit(sortSet, ShellSort);
      break;
    }
    case 'q': {
      setBit(sortSet, QuickSort);
      break;
    }
    case 'M': {
      setBit(sortSet, MergeSort);
      break;
    }
    case 'Q': {
      setBit(sortSet, QSI);
      break;
    }
    case 'h': {
      setBit(sortSet, HeapSort);
      break;
    }
    case 'p': {
      printMax = atoi(optarg);
      break;
    }
    case 'r': {
      seed = atoi(optarg);
      break;
    }
    case 'n': {
      count = atoi(optarg);
      break;
    }
    }
  }

  a = calloc(count, sizeof(uint32_t));

  if (valBit(sortSet, EndSort)) {
    SRANDOM(seed); // Where shall we start?
    fillArray(a, count, valBit(sortSet, Ordered)); // Give us numbers
    printf("Unsorted\n");
    printArray(a, count); // Show us
  }

  for (enum sorts s = MinSort; s < EndSort; s += 1) {
    if (valBit(sortSet, s) == 1) {
      compares = 0;
      moves = 0; // Reset statistics
      SRANDOM(seed); // Where shall we start?
      fillArray(a, count, valBit(sortSet, Ordered)); // Give us numbers

      switch (s) {
      case MinSort: {
        printf("Min Sort\n");
        minSort(a, count);
        break;
      }
      case BubbleSort: {
        printf("Bubble Sort\n");
        bubbleSort(a, count);
        break;
      }
      case ShakerSort: {
        printf("Shaker Sort\n");
        shakerSort(a, count);
        break;
      }
      case InsertionSort: {
        printf("Insertion Sort\n");
        insertionSort(a, count);
        break;
      }
      case ShellSort: {
        printf("Shell Sort\n");
        shellSort(a, count);
        break;
      }
      case QuickSort: {
        printf("Quick Sort\n");
        qSort(a, count);
        break;
      }
      case QSI: {
        printf("Quick Sort (Iterative)\n");
        qSortI(a, count);
        break;
      }
      case MergeSort: {
        printf("Merge Sort\n");
        mergeSort(a, count);
        break;
      }
      case HeapSort: {
        printf("Heap Sort\n");
        heapSort(a, count);
        break;
      }
      default: break;
      }
      printf("%u elements\n", count);
      printf("%llu moves\n", moves);
      printf("%llu compares\n", compares);
      printArray(a, count);
    }
  }
  free(a);
  delVec(sortSet);
  return 0;
}
