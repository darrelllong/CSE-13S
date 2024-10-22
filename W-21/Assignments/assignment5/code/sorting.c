#include "bubble.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "util.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(char *program) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   A collection of comparison-based sorting algorithms.\n"
        "\n"
        "USAGE\n"
        "   %s [-absqh] [-n length] [-p elements] [-r seed]\n"
        "\n"
        "OPTIONS\n"
        "   -a              Enable all sorts.\n"
        "   -b              Enable Bubble Sort.\n"
        "   -s              Enable Shell Sort.\n"
        "   -q              Enable Quick Sort.\n"
        "   -h              Enable Heap Sort.\n"
        "   -n length       Specify number of array elements.\n"
        "   -p elements     Specify number of elements to print.\n"
        "   -r seed         Specify random seed.\n",
        program);
}

#define LENGTH    100
#define ELEMENTS  100
#define SEED      7092016
#define OPTIONS   "absqhn:p:r:"


int main(int argc, char **argv) {
    typedef enum sorts { BUBBLE, SHELL, QUICK, HEAP } sorts;

    int opt = 0;
    sorts set = 0;
    uint32_t seed = SEED;
    uint32_t length = LENGTH;
    uint32_t elements = ELEMENTS;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': { set = set_all(set); break; }
        case 'b': { set = set_insert(set, BUBBLE); break; }
        case 's': { set = set_insert(set, SHELL); break; }
        case 'q': { set = set_insert(set, QUICK); break; }
        case 'h': { set = set_insert(set, HEAP); break; }
        case 'n':
            length = (uint32_t) strtoul(optarg, NULL, 10);
            check(!length || errno == EINVAL, "Invalid array length.\n");
            break;
        case 'p':
            elements = (uint32_t) strtoul(optarg, NULL, 10);
            check(!length || errno == EINVAL, "Invalid print number.\n");
            break;
        case 'r':
            seed = (uint32_t) strtoul(optarg, NULL, 10);
            check(errno == EINVAL, "Invalid random seed.\n");
            break;
        default:
            usage(argv[0]);
            return 1;
        }
    }

    if (!set) {
        fprintf(stderr, "Select at least one sort to perform.\n");
        usage(argv[0]);
        return 1;
    }

    uint32_t *A = (uint32_t *) calloc(length, sizeof(uint32_t));
    check(!A, "Failed to allocate array to sort.\n");

    void (*table[4])(uint32_t *, uint32_t) = {
        bubble_sort, shell_sort, quick_sort, heap_sort
    };

    const char *names[] = {
        "Bubble", "Shell", "Quick", "Heap"
    };

    for (sorts s = BUBBLE; s <= HEAP; s += 1) {
        if (set_member(set, s)) {
            reset_stats();
            fill_array(A, length, seed);
            table[s](A, length);
            printf("%s Sort\n", names[s]);
            print_stats(length);
            print_array(A, length < elements ? length : elements);
        }
    }

    free(A);
    return 0;
}
