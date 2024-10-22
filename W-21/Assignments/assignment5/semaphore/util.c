#include "util.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define COLUMNS 7
#define MASK    0x3FFFFFFF

static clock_t start;
static clock_t stop;

bool timed = false;
bool ordered = false;
uint64_t moves = 0;
uint64_t compares = 0;

void swap(uint32_t *x, uint32_t *y) {
  uint32_t temp = *x;
  *x = *y;
  *y = temp;
  moves += 3;
  return;
}

bool compare(uint32_t x, uint32_t y) {
  compares += 1;
  return x < y;
}

void start_timer(void) {
  start = clock();
  return;
}

void stop_timer(void) {
  stop = clock();
  return;
}

void print_stats(uint32_t elements) {
  printf("%" PRIu32 " elements, "
    "%" PRIu64 " moves, "
    "%" PRIu64 " compares",
    elements, moves, compares);

  if (timed) {
    printf(", %lf seconds\n", (stop - start) / (double)CLOCKS_PER_SEC);
  } else {
    printf("\n");
  }

  return;
}

void print_array(uint32_t *arr, uint32_t elements) {
  if (elements == 0) {
    return;
  }

  for (uint32_t i = 0; i < elements; i += 1) {
    if (i && (i % COLUMNS == 0)) {
      printf("\n");
    }

    printf("%13d", arr[i]);
  }

  printf("\n");
  return;
}

uint32_t *filled_array(uint32_t length, uint32_t seed) {
  uint32_t *arr = (uint32_t *)malloc(length * sizeof(uint32_t));
  check(arr, "Array not allocated!\n");

  srand(seed);

  for (uint32_t i = 0; i < length; i += 1) {
    arr[i] = ordered ? i : rand() & MASK;
  }

  return arr;
}

void check(bool cond, const char *format, ...) {
  if (!cond) {
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);
    exit(EXIT_FAILURE);
  }

  return;
}

