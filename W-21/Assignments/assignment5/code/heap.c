#include "heap.h"
#include "util.h"

static uint32_t max_child(uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = first * 2;
    uint32_t right = left + 1;
    uint32_t child = left;

    if (right <= last && compare(A[left - 1], A[right - 1])) {
        child = right;
    }

    return child;
}

static void fix_heap(uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t father = first;
    uint32_t great = max_child(A, father, last);

    while (father <= last / 2 && !found) {
        if (compare(A[father - 1], A[great - 1])) {
            swap(&A[father - 1], &A[great - 1]);
            father = great;
            great = max_child(A, father, last);
        } else {
            found = true;
        }
    }
}

static void build_heap(uint32_t *A, uint32_t first, uint32_t last) {
    for (int64_t father = last / 2; father >= first; father -= 1) {
        fix_heap(A, father, last);
    }
}

void heap_sort(uint32_t *A, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(A, first, last);
    for (uint32_t leaf = last; leaf >= first + 1; leaf -= 1) {
        swap(&A[first - 1], &A[leaf - 1]);
        fix_heap(A, first, leaf - 1);
    }
}
