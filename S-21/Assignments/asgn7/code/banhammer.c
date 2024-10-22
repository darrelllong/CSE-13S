#include "bf.h"
#include "ht.h"
#include "messages.h"
#include "parser.h"

#include <ctype.h>
#include <inttypes.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(FILE *stream, char *exec) {
    fprintf(stream,
            "SYNOPSIS\n"
            "  A word filtering program for the GPRSC.\n"
            "  Filters out and reports bad words parsed from stdin.\n"
            "\n"
            "USAGE\n"
            "  %s [-hsm] [-t size] [-f size]\n"
            "\n"
            "OPTIONS\n"
            "  -h           Program usage and help.\n"
            "  -s           Print program statistics.\n"
            "  -m           Enable move-to-front rule.\n"
            "  -t size      Specify hash table size (default: 10000).\n"
            "  -f size      Specify Bloom filter size (default: 2^20).\n",
            exec);
}

#define BLOCK 4096
#define BADSPEAK "badspeak.txt"
#define NEWSPEAK "newspeak.txt"

void load_badspeak(BloomFilter *b, HashTable *h) {
    FILE *infile = fopen(BADSPEAK, "r");
    if (!infile) {
        fprintf(stderr, "Failed to open badspeak.txt.\n");
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
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

#define HT_SIZE 10000
#define BF_SIZE (1 << 20)
#define OPTIONS "t:f:smh"
#define WORD "[a-zA-Z0-9_]+(('|-)[a-zA-Z0-9_]+)*"

int main(int argc, char **argv) {
    int c = 0;
    bool mtf = false;
    bool stats = false;
    uint32_t bf_size = BF_SIZE;
    uint32_t ht_size = HT_SIZE;

    while ((c = getopt(argc, argv, OPTIONS)) != -1) {
        switch (c) {
        case 't':
            ht_size = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'f':
            bf_size = (uint32_t)strtoul(optarg, NULL, 10);
            break;
        case 'm':
            mtf = true;
            break;
        case 's':
            stats = true;
            break;
        case 'h':
            usage(stdout, argv[0]);
            return EXIT_SUCCESS;
        default:
            usage(stderr, argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (bf_size == 0) {
        fprintf(stderr, "Invalid Bloom filter size.\n");
        return EXIT_FAILURE;
    }

    if (ht_size == 0) {
        fprintf(stderr, "Invalid hash table size.\n");
        return EXIT_FAILURE;
    }

    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        perror("regcomp");
        return EXIT_FAILURE;
    }

    BloomFilter *b = bf_create(bf_size);
    if (!b) {
        fprintf(stderr, "Failed to create Bloom filter.\n");
        return EXIT_FAILURE;
    }

    HashTable *h = ht_create(ht_size, mtf);
    if (!h) {
        fprintf(stderr, "Failed to create hash table.\n");
        bf_delete(&b);
        regfree(&re);
        return EXIT_FAILURE;
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

    if (stats) {
        double avg = (double)links / (double)seeks;
        double ht_load_ratio = (double)ht_count(h) / (double)ht_size;
        double bf_load_ratio = (double)bf_count(b) / (double)bf_size;
        printf("Seeks: %" PRIu64 "\n", seeks);
        printf("Average seek length: %lf\n", avg);
        printf("Hash table load: %lf%%\n", 100 * ht_load_ratio);
        printf("Bloom filter load: %lf%%\n", 100 * bf_load_ratio);
    } else {
        if (ll_length(revise) > 0 && ll_length(bad) == 0) {
            printf("%s", goodspeak_message);
            ll_print(revise);
        } else if (ll_length(revise) == 0 && ll_length(bad) > 0) {
            printf("%s", badspeak_message);
            ll_print(bad);
        } else if (ll_length(revise) > 0 && ll_length(bad) > 0) {
            printf("%s", mixspeak_message);
            ll_print(bad);
            ll_print(revise);
        }
    }

    bf_delete(&b);
    ht_delete(&h);
    ll_delete(&bad);
    ll_delete(&revise);
    regfree(&re);
    clear_words();
    return EXIT_SUCCESS;
}
