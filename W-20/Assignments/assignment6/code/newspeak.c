#include "bf.h"
#include "gs.h"
#include "hash.h"
#include "ll.h"
#include "parser.h"
#include "util.h"
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK     4096
#define HT_SIZE   10000
#define BF_SIZE   1 << 20
#define BADSPEAK  "badspeak.txt"
#define NEWSPEAK  "newspeak.txt"
#define OPTIONS   "h:f:smb"
#define WORD      "[a-zA-Z0-9_]+(('|-)[a-zA-Z0-9_]+)*"

extern bool move_to_front;
extern uint64_t links;
extern uint64_t seeks;

void print_program_usage(char **argv) {
  fprintf(stderr,
    "Usage: %s -mb [-hf]\n"
    "   -s: print statistics.\n"
    "   -m: enable move-to-front rule.\n"
    "   -b: disable move-to-front rule.\n"
    "   -h <size>: set hash table size.\n"
    "   -f <size>: set Bloom filter size.\n"
    "   -i <file>: set file to censor.\n",
    argv[0]);
  return;
}

void print_thoughtcrime_letter(void) {
  printf("Dear Comrade,\n\n"
    "You have chosen to use degenerate words that may cause hurt\n"
    "feelings or cause your comrades to think unpleasant thoughts.\n"
    "This is doubleplus bad. To correct your wrongthink and\n"
    "preserve community consensus we will be sending you to joycamp\n"
    "administered by Miniluv.\n\n"
    "Your errors:\n\n");
  return;
}

void print_newspeak_letter(void) {
  printf("Dear Comrade,\n\n"
    "Submitting your text helps to preserve feelings and prevent\n"
    "badthink. Some of the words that you used are not goodspeak.\n"
    "The list shows how to turn the oldspeak words into newspeak.\n\n");
  return;
}

void load_badspeak(BloomFilter *b, HashTable *h) {
  FILE *infile = fopen(BADSPEAK, "r");
  check(infile, "Failed to open \"%s\"!\n", BADSPEAK);

  char badspeak[BLOCK];

  while (fscanf(infile, "%s\n", badspeak) != EOF) {
    GoodSpeak *g = gs_create(badspeak, NULL);
    bf_insert(b, gs_oldspeak(g));
    ht_insert(h, g);
  }

  fclose(infile);
  return;
}

void load_newspeak(BloomFilter *b, HashTable *h) {
  FILE *infile = fopen(NEWSPEAK, "r");
  check(infile, "Failed to open \"%s\"!\n", NEWSPEAK);

  char oldspeak[BLOCK];
  char newspeak[BLOCK];

  while (fscanf(infile, "%s %s\n", oldspeak, newspeak) != EOF) {
    GoodSpeak *g = gs_create(oldspeak, newspeak);
    bf_insert(b, gs_oldspeak(g));
    ht_insert(h, g);
  }

  fclose(infile);
  return;
}

int main(int argc, char **argv) {
  int c = 0;
  bool stats = false;
  int64_t bf_size = BF_SIZE;
  int64_t ht_size = HT_SIZE;

  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'h':
      ht_size = (int64_t)strtoull(optarg, NULL, 10);
      check(ht_size > 0, "Negative hash table size?!\n");
      check(ht_size <= UINT32_MAX, "Hash table size too big!\n");
      break;
    case 'f':
      bf_size = (int64_t)strtoull(optarg, NULL, 10);
      check(bf_size > 0, "Negative Bloom filter size?!\n");
      check(bf_size <= UINT32_MAX, "Bloom filter size too big!\n");
      break;
    case 'm':
      move_to_front = true;
      break;
    case 'b':
      move_to_front = false;
      break;
    case 's':
      stats = true;
      break;
    default:
      print_program_usage(argv);
      exit(1);
    }
  }

  BloomFilter *b = bf_create(bf_size);
  HashTable *h = ht_create(ht_size);

  load_badspeak(b, h);
  load_newspeak(b, h);

  regex_t re;
  char *word = NULL;
  ListNode *revise = NULL;
  ListNode *forbidden = NULL;

  if (regcomp(&re, WORD, REG_EXTENDED)) {
    perror("regcomp");
    exit(1);
  }

  while ((word = next_word(stdin, &re))) {
    str_lowercase(word);

    if (bf_probe(b, word)) {
      ListNode *n = ht_lookup(h, word);
      if (n) {
        char *oldspeak = gs_oldspeak(ll_node_gs(n));
        char *newspeak = gs_newspeak(ll_node_gs(n));
        GoodSpeak *g = gs_create(oldspeak, newspeak);

        if (gs_newspeak(g)) {
          revise = ll_insert(&revise, g);
        } else {
          forbidden = ll_insert(&forbidden, g);
        }
      }
    }
  }

  if (stats) {
    if (seeks > 0) {
      double avg = (double)links / (double)seeks;
      double ht_load_ratio = (double)ht_count(h) / (double)ht_size;
      double bf_load_ratio = (double)bf_count(b) / (double)bf_size;

      printf("Seeks: %" PRIu64 "\n", seeks);
      printf("Average seek length: %lf\n", avg);
      printf("Hash table load: %lf%%\n", 100 * ht_load_ratio);
      printf("Bloom filter load: %lf%%\n", 100 * bf_load_ratio);
    } else {
      printf("No searches performed!\n");
    }
  } else {
    if (revise && !forbidden) {
      print_newspeak_letter();
      ll_print(revise);
    } else if (!revise && forbidden) {
      print_thoughtcrime_letter();
      ll_print(forbidden);
    } else if (revise && forbidden) {
      print_thoughtcrime_letter();
      ll_print(forbidden);
      printf("\nThink on these words during your vacation!\n\n");
      ll_print(revise);
    }
  }

  bf_delete(b);
  ht_delete(h);
  ll_delete(forbidden);
  ll_delete(revise);
  regfree(&re);
  clear_words();

  return 0;
}
