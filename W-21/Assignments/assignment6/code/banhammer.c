#include "bf.h"
#include "hash.h"
#include "parser.h"
#include <ctype.h>
#include <inttypes.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(char **argv) {
    fprintf(stderr,
        "Usage: %s [-m] [-h size] [-f size]\n"
        "   -s          Print statistics.\n"
        "   -m          Enable move-to-front rule.\n"
        "   -h size     Set hash table size.\n"
        "   -f size     Set Bloom filter size.\n",
        argv[0]);
}

void print_thoughtcrime_letter(void) {
    printf("Dear Comrade,\n\n"
            "You have chosen to use degenerate words that may cause hurt\n"
            "feelings or cause your comrades to think unpleasant thoughts.\n"
            "This is doubleplus bad. To correct your wrongthink and\n"
            "preserve community consensus we will be sending you to joycamp\n"
            "administered by Medellin's Miniluv. Beware of the hippos\n\n"
            "Your errors:\n\n");
}

void print_newspeak_letter(void) {
    printf("Dear Comrade,\n\n"
            "Submitting your text helps to preserve feelings and prevent\n"
            "badthink. Some of the words that you used are not goodspeak.\n"
            "The list shows how to turn the oldspeak words into newspeak.\n\n");
}

#define BLOCK     4096
#define BADSPEAK  "badspeak.txt"
#define NEWSPEAK  "newspeak.txt"

void load_badspeak(BloomFilter *b, HashTable *h) {
    FILE *infile = fopen(BADSPEAK, "r");
    if (!infile) {
        fprintf(stderr, "Failed to open badspeak.txt.\n");
        exit(1);
    }

    char badspeak[BLOCK];

    while (fscanf(infile, "%s\n", badspeak) != EOF) {
        bf_insert(b, badspeak);
        ht_insert(h, badspeak, NULL);
    }

    fclose(infile);
}

void load_newspeak(BloomFilter *b, HashTable *h) {
    FILE *infile = fopen(NEWSPEAK, "r");
    if (!infile) {
        fprintf(stderr, "Failed to open newspeak.txt.\n");
        exit(1);
    }

    char oldspeak[BLOCK];
    char newspeak[BLOCK];

    while (fscanf(infile, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(b, oldspeak);
        ht_insert(h, oldspeak, newspeak);
    }

    fclose(infile);
}

void lowercase(char *str) {
    for (uint64_t i = 0; i < strlen(str); i += 1) {
        str[i] = tolower(str[i]);
    }
}

#define HT_SIZE   10000
#define BF_SIZE   1 << 20
#define OPTIONS   "h:f:sm"
#define WORD      "[a-zA-Z0-9_]+(('|-)[a-zA-Z0-9_]+)*"

int main(int argc, char **argv) {
    int c = 0;
    bool mtf = false;
    bool stats = false;
    uint32_t bf_size = BF_SIZE;
    uint32_t ht_size = HT_SIZE;

    while ((c = getopt(argc, argv, OPTIONS)) != -1) {
        switch (c) {
        case 'h':
            ht_size = (uint32_t) strtoul(optarg, NULL, 10);
            break;
        case 'f':
            bf_size = (uint32_t) strtoul(optarg, NULL, 10);
            break;
        case 'm':
            mtf = true;
            break;
        case 's':
            stats = true;
            break;
        default:
            usage(argv);
            return 1;
        }
    }

    if (bf_size == 0) {
        fprintf(stderr, "Invalid Bloom filter size.\n");
        exit(1);
    }

    if (ht_size == 0) {
        fprintf(stderr, "Invalid hash table size.\n");
        exit(1);
    }

    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        perror("regcomp");
        return 1;
    }

    BloomFilter *b = bf_create(bf_size);
    if (!b) {
        fprintf(stderr, "Failed to create Bloom filter.\n");
        exit(1);
    }

    HashTable *h = ht_create(ht_size, mtf);
    if (!h) {
        fprintf(stderr, "Failed to create hash table.\n");
        bf_delete(&b);
        regfree(&re);
        exit(1);
    }

    load_badspeak(b, h);
    load_newspeak(b, h);

    char *word = NULL;
    LinkedList *revise = ll_create(mtf);
    LinkedList *bad = ll_create(mtf);

    while ((word = next_word(stdin, &re))) {
        lowercase(word);
        if (bf_probe(b, word)) {
            Node *n = ht_lookup(h, word);
            if (n) {
                ll_insert(n->newspeak ? revise : bad, n->oldspeak, n->newspeak);
            }
        }
    }

    if (ll_length(revise) > 0 && ll_length(bad) == 0) {
        print_newspeak_letter();
        ll_print(revise);
    } else if (ll_length(revise) == 0 && ll_length(bad) > 0) {
        print_thoughtcrime_letter();
        ll_print(bad);
    } else if (ll_length(revise) > 0 && ll_length(bad) > 0) {
        print_thoughtcrime_letter();
        ll_print(bad);
        printf("\nThink on these words on your vacation!\n\n");
        ll_print(revise);
    }

    bf_delete(&b);
    ht_delete(&h);
    ll_delete(&bad);
    ll_delete(&revise);
    regfree(&re);
    clear_words();
    return 0;
}
