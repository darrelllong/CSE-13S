#include "util.h"
#include "quick.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define COLUMNS 5
#define MASK    0x3FFFFFFF

uint64_t moves = 0;
uint64_t compares = 0;

void swap(uint32_t *x, uint32_t *y) {
    uint32_t z = *x;
    *x = *y;
    *y = z;
    moves += 3;
    return;
}

bool compare(uint32_t x, uint32_t y) {
    compares += 1;
    return x < y;
}

void reset_stats(void) {
    moves = 0;
    compares = 0;
}

void print_stats(uint32_t elements, Sorts s) {
    printf("%" PRIu32 " elements, "
           "%" PRIu64 " moves, "
           "%" PRIu64 " compares\n",
        elements, moves, compares);
    switch (s) {
    case QUICKSTACK:
        printf("Max stack size: %" PRIu32 "\n", max_stack_size);
        break;
    case QUICKQUEUE:
        printf("Max queue size: %" PRIu32 "\n", max_queue_size);
        break;
    default:
        break;
    }
}

void print_array(uint32_t *arr, uint32_t elements) {
    if (elements) {
        for (uint32_t i = 0; i < elements; i += 1) {
            if (i && (i % COLUMNS == 0)) {
                printf("\n");
            }
            printf("%13d", arr[i]);
        }
        printf("\n");
    }
}

void fill_array(uint32_t *A, uint32_t n, uint32_t seed, bool ordered) {
    srandom(seed);
    for (uint32_t i = 0; i < n; i += 1) {
        A[i] = ordered ? i : random() & MASK;
    }
}

void check(bool cond, const char *fmt, ...) {
    if (cond) {
        va_list arg;
        va_start(arg, fmt);
        vfprintf(stderr, fmt, arg);
        va_end(arg);
        exit(EXIT_FAILURE);
    }
}

