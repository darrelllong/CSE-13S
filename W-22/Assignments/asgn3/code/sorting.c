#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "batcher.h"
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "   A collection of comparison-based sorting algorithms.\n"
            "\n"
            "USAGE\n"
            "   %s [-Haibhqn:p:r:] [-n length] [-p elements] [-r seed]\n"
            "\n"
            "OPTIONS\n"
            "   -H              Display program help and usage.\n"
            "   -a              Enable all sorts.\n"
            "   -i              Enable Insertion Sort.\n"
            "   -b              Enable Batcher Sort.\n"
            "   -h              Enable Heap Sort.\n"
            "   -q              Enable Quick Sort.\n"
            "   -n length       Specify number of array elements (default: 100).\n"
            "   -p elements     Specify number of elements to print (default: 100).\n"
            "   -r seed         Specify random seed (default: 13371453).\n",
            exec);
}

#define COLUMNS  5
#define LENGTH   100
#define ELEMENTS 100
#define SEED     13371453
#define MASK     0x3FFFFFFF
#define OPTIONS  "Haibhqon:p:r:"

#define SORTS_LIST                                            \
    X(INSERTION, insertion_sort, "Insertion Sort"),           \
    X(BATCHER, batcher_sort, "Batcher Sort"),                 \
    X(HEAP, heap_sort, "Heap Sort"),                          \
    X(QUICK, quick_sort, "Quick Sort"),                       \

#define X(sort, fn, name) sort
typedef enum { SORTS_LIST } Sorts;
#undef X

#define X(sort, fn, name) fn
void (*fntable[])(Stats *, uint32_t *, uint32_t) = { SORTS_LIST };
#undef X

#define X(sort, fn, name) name
const char *names[] = { SORTS_LIST };
#undef X

void print_stats(Stats *stats, const char *sort, uint32_t elements) {
    printf("%s, %" PRIu32 " elements, " "%" PRIu64 " moves, " "%" PRIu64 " compares\n",
        sort, elements, stats->moves, stats->compares);
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

int main(int argc, char **argv) {
    int opt = 0;
    Sorts set = 0;
    bool ordered = false;
    uint32_t seed = SEED, length = LENGTH, elements = ELEMENTS;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            set = set_complement(set_empty());
            break;
        case 'i':
            set = set_insert(set, INSERTION);
            break;
        case 'b':
            set = set_insert(set, BATCHER);
            break;
        case 'h':
            set = set_insert(set, HEAP);
            break;
        case 'q':
            set = set_insert(set, QUICK);
            break;
        case 'o':
            ordered = true;
            break;
        case 'n':
            length = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'p':
            elements = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'r':
            seed = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'H': {
            usage(argv[0]);
            return EXIT_SUCCESS;
        }
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (set == set_empty()) {
        fprintf(stderr, "Select at least one sort to perform.\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    uint32_t *A = (uint32_t *)calloc(!length ? 1 : length, sizeof(uint32_t));

    for (Sorts s = INSERTION; s <= QUICK; s += 1) {
        if (set_member(set, s)) {
            Stats stats = { 0 };
            fill_array(A, length, seed, ordered);
            fntable[s](&stats, A, length);
            print_stats(&stats, names[s], length);
            print_array(A, length < elements ? length : elements);
        }
    }

    free(A);
    return EXIT_SUCCESS;
}
