#include "binary.h"
#include "bubble.h"
#include "cocktail.h"
#include "insert.h"
#include "merge.h"
#include "quick.h"
#include "schlub.h"
#include "select.h"
#include "shell.h"
#include "semwrapper.h"
#include "util.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ALL       0xFFFF
#define LENGTH    100
#define ELEMENTS  100
#define SEED      8222022
#define OPTIONS   "Absqicxmelothn:p:r:"

extern bool timed;
extern bool ordered;

typedef enum sorts {
  SELECT =   0x1,   BUBBLE =  0x2,  INSERT =  0x4,  SHELL    =  0x8,
  QUICK  =  0x10,   MERGE  = 0x20,  BINARY = 0x40,  COCKTAIL = 0x80,
  SCHLUB = 0x100
} sorts;

void print_sort_name(sorts s) {
  switch (s) {
  case SELECT:    { printf("Selection Sort\n");         break; }
  case BUBBLE:    { printf("Bubble Sort\n");            break; }
  case INSERT:    { printf("Insertion Sort\n");         break; }
  case SHELL:     { printf("Shell Sort\n");             break; }
  case QUICK:     { printf("Quick Sort\n");             break; }
  case MERGE:     { printf("Merge Sort\n");             break; }
  case BINARY:    { printf("Binary Insertion Sort\n");  break; }
  case SCHLUB:    { printf("Schlub Sort\n");            break; }
  case COCKTAIL:  { printf("Cocktail Sort\n");          break; }
  default:        { break; }
  }

  return;
}

void print_usage(char **argv) {
  fprintf(stderr,
    "SYNOPSIS\n"
    "   A collection of comparison-based sorting algorithms.\n"
    "\n"
    "USAGE\n"
    "   %s [-Absqicxmelhot] [-n length] [-p elements] [-r seed]\n"
    "\n"
    "OPTIONS\n"
    "   -A              Enable all sorts.\n"
    "   -b              Enable Bubble Sort.\n"
    "   -s              Enable Shell Sort.\n"
    "   -q              Enable Quick Sort.\n"
    "   -i              Enable Binary Insertion Sort.\n"
    "   -c              Enable Cocktail Sort.\n"
    "   -x              Enable Insertion Sort.\n"
    "   -m              Enable Merge Sort.\n"
    "   -e              Enable Selection Sort.\n"
    "   -l              Enable Schlub Sort.\n"
    "   -h              Show program help and usage.\n"
    "   -o              Specify ordered arrays.\n"
    "   -t              Time sorting of arrays.\n"
    "   -n length       Specify number of array elements.\n"
    "   -p elements     Specify number of elements to print.\n"
    "   -r seed         Specify random seed.\n",
    argv[0]);
  return;
}

int main(int argc, char **argv) {
  int c = 0;
  uint16_t sort_set = 0;
  uint32_t seed = SEED;
  int64_t length = LENGTH;
  int64_t elements = ELEMENTS;

  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'A': { sort_set |= ALL;      break; }
    case 'b': { sort_set |= BUBBLE;   break; }
    case 's': { sort_set |= SHELL;    break; }
    case 'q': { sort_set |= QUICK;    break; }
    case 'i': { sort_set |= BINARY;   break; }
    case 'c': { sort_set |= COCKTAIL; break; }
    case 'e': { sort_set |= SELECT;   break; }
    case 'm': { sort_set |= MERGE;    break; }
    case 'x': { sort_set |= INSERT;   break; }
    case 'l': { sort_set |= SCHLUB;   break; }
    case 'o':
      ordered = true;
      break;
    case 't':
      timed = true;
      break;
    case 'n':
      length = (int64_t)strtoull(optarg, NULL, 10);
      check(length > 0, "Can't have negative array length!\n");
      check(length <= UINT32_MAX, "Too many elements!\n");
      break;
    case 'p':
      elements = (int64_t)strtoull(optarg, NULL, 10);
      check(length > 0, "Can't print negative elements!\n");
      break;
    case 'r':
      seed = (uint32_t)strtol(optarg, NULL, 10);
      break;
    case 'h':
      print_usage(argv);
      exit(EXIT_SUCCESS);
    default:
      print_usage(argv);
      exit(EXIT_FAILURE);
    }
  }

  if (!sort_set) {
    fprintf(stderr, "Select at least one sort to perform!\n\n");
    print_usage(argv);
    exit(EXIT_FAILURE);
  }

  elements = (length < elements) ? length : elements;

  int sem = sem_create();
  uint32_t *arr = filled_array(length, seed);

  for (sorts s = SCHLUB; s; s >>= 1) {
    if (sort_set & s) {
      switch (fork()) {
      case -1:
        perror("fork");
        exit(EXIT_FAILURE);
      case 0:
        start_timer();

        switch (s) {
        case SELECT:    { selection_sort(arr, length);        break; }
        case BUBBLE:    { bubble_sort(arr, length);           break; }
        case INSERT:    { insertion_sort(arr, length);        break; }
        case SHELL:     { shell_sort(arr, length);            break; }
        case QUICK:     { quick_sort(arr, length);            break; }
        case MERGE:     { merge_sort(arr, 0, length - 1);     break; }
        case BINARY:    { binary_insertion_sort(arr, length); break; }
        case COCKTAIL:  { cocktail_sort(arr, length);         break; }
        case SCHLUB:    { schlub_sort(arr, length);           break; }
        default:        { break; }
        }

        stop_timer();

        sem_wait(sem);
        print_sort_name(s);
        print_stats(length);
        print_array(arr, elements);
        sem_signal(sem);

        free(arr);
        exit(EXIT_SUCCESS);
      default:
        break;
      }
    }
  }

  while (wait(NULL) > 0) ;

  sem_delete(sem);
  free(arr);

  return EXIT_SUCCESS;
}
