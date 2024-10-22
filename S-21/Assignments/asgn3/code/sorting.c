#include "bubble.h"
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
            "   %s [-habsqQo] [-n length] [-p elements] [-r seed]\n"
            "\n"
            "OPTIONS\n"
            "   -h              Display program help and usage.\n"
            "   -a              Enable all sorts.\n"
            "   -b              Enable Bubble Sort.\n"
            "   -s              Enable Shell Sort.\n"
            "   -q              Enable Quick Sort (Stack).\n"
            "   -Q              Enable Quick Sort (Queue).\n"
            "   -n length       Specify number of array elements.\n"
            "   -p elements     Specify number of elements to print.\n"
            "   -r seed         Specify random seed.\n"
            "   -o              Use sorted arrays.\n",
            program);
}

#define LENGTH   100
#define ELEMENTS 100
#define SEED     13371453
#define OPTIONS  "habsqQon:p:r:"

int main(int argc, char **argv) {
    int opt = 0;
    Sorts set = 0;
    bool ordered = false;
    uint32_t seed = SEED;
    uint32_t length = LENGTH;
    uint32_t elements = ELEMENTS;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': {
            set = set_complement(set_empty());
            break;
        }
        case 'b': {
            set = set_insert(set, BUBBLE);
            break;
        }
        case 's': {
            set = set_insert(set, SHELL);
            break;
        }
        case 'q': {
            set = set_insert(set, QUICKSTACK);
            break;
        }
        case 'Q': {
            set = set_insert(set, QUICKQUEUE);
            break;
        }
        case 'o': {
            ordered = true;
            break;
        }
        case 'n':
            length = (uint32_t)strtoul(optarg, NULL, 10);
            check(!length || errno == EINVAL, "Invalid array length.\n");
            break;
        case 'p':
            elements = (uint32_t)strtoul(optarg, NULL, 10);
            check(!length || errno == EINVAL, "Invalid print number.\n");
            break;
        case 'r':
            seed = (uint32_t)strtoul(optarg, NULL, 10);
            check(errno == EINVAL, "Invalid random seed.\n");
            break;
        case 'h': {
            usage(argv[0]);
            return EXIT_SUCCESS;
        }
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (!set) {
        fprintf(stderr, "Select at least one sort to perform.\n");
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    uint32_t *A = (uint32_t *)calloc(!length ? 1 : length, sizeof(uint32_t));
    check(!A, "Failed to allocate array to sort.\n");

    void (*table[4])(uint32_t *, uint32_t) = {
        [BUBBLE] = bubble_sort,
        [SHELL] = shell_sort,
        [QUICKSTACK] = quick_sort_stack,
        [QUICKQUEUE] = quick_sort_queue,
    };

    const char *names[] = {
        [BUBBLE] = "Bubble Sort",
        [SHELL] = "Shell Sort",
        [QUICKSTACK] = "Quick Sort (Stack)",
        [QUICKQUEUE] = "Quick Sort (Queue)",
    };

    for (Sorts s = BUBBLE; s < SORTS; s += 1) {
        if (set_member(set, s)) {
            reset_stats();
            fill_array(A, length, seed, ordered);
            table[s](A, length);
            printf("%s\n", names[s]);
            print_stats(length, s);
            print_array(A, length < elements ? length : elements);
        }
    }

    free(A);
    return EXIT_SUCCESS;
}
