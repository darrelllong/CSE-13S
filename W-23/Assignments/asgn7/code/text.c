#include "text.h"

#include "bf.h"
#include "ht.h"
#include "metric.h"
#include "parser.h"

#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#define WORD "[a-zA-Z]+(('|-)[a-zA-Z]+)*"

uint32_t noiselimit = 100;

struct Text {
    BloomFilter *filter;
    HashTable *words;
    uint32_t count;
};

static inline void lowercase(char *str) {
    for (uint32_t i = 0; str[i]; i += 1) {
        str[i] = tolower(str[i]);
    }
}

Text *text_create(FILE *infile, Text *noise) {
    Text *text = (Text *) malloc(sizeof(Text));
    text->filter = bf_create(1 << 21); // Maybe use some heuristic for the size...
    text->words = ht_create(1 << 19); // Maybe use some heuristic for the size...
    text->count = 0;

    regex_t re;
    regcomp(&re, WORD, REG_EXTENDED);

    char *word = NULL;
    while ((word = next_word(infile, &re)) != NULL) {
        lowercase(word);

        // Only add the word if it isn't considered noise.
        // But add words if we're populating the noise text.
        if (noise && bf_probe(noise->filter, word) && ht_lookup(noise->words, word)) {
            continue;
        }
        bf_insert(text->filter, word);
        ht_insert(text->words, word);
        text->count += 1;

        // Stop if we're populating the noise text and hit the limit.
        if (!noise && text->count == noiselimit) {
            break;
        }
    }

    regfree(&re);
    return text;
}

void text_delete(Text **text) {
    if (*text) {
        bf_delete(&(*text)->filter);
        ht_delete(&(*text)->words);
        free(*text);
        *text = NULL;
    }
}

double text_dist(Text *text1, Text *text2, Metric metric) {
    double sum = 0.0, diff = 0.0;
    Node *n = NULL, *m = NULL;
    double n_freq = 0, m_freq = 0;
    HashTableIterator *hti1 = hti_create(text1->words);
    HashTableIterator *hti2 = hti_create(text2->words);

    // Iterate over text1 and check in text2.
    while ((n = ht_iter(hti1)) != NULL) {
        n_freq = text_frequency(text1, n->word);
        m_freq = text_frequency(text2, n->word);

        switch (metric) {
        case EUCLIDEAN:
            diff = n_freq - m_freq;
            sum += diff * diff;
            break;
        case MANHATTAN:
            diff = n_freq - m_freq;
            sum += fabs(diff);
            break;
        case COSINE:
            diff = n_freq * m_freq;
            sum += diff;
            break;
        }
    }

    // Iterate over text2, only considering what's in text2 but not text1.
    // It's possible that the Bloom filter reports a false positive due to hash
    // collisions, so in the case where the filter reports the word is in the
    // hash table, we still need to check.
    while ((m = ht_iter(hti2)) != NULL) {
        if (!text_contains(text1, m->word)) {
            n_freq = 0;
            m_freq = text_frequency(text2, m->word);

            switch (metric) {
            case EUCLIDEAN:
                diff = n_freq - m_freq;
                sum += diff * diff;
                break;
            case MANHATTAN:
                diff = n_freq - m_freq;
                sum += fabs(diff);
                break;
            case COSINE:
                diff = n_freq * m_freq;
                sum += diff;
                break;
            }
        }
    }

    switch (metric) {
    case EUCLIDEAN: sum = sqrt(sum); break;
    case COSINE: sum = 1.0 - sum; break;
    default: break;
    }

    // Clean up the iterators.
    hti_delete(&hti1);
    hti_delete(&hti2);

    return sum;
}

double text_frequency(Text *text, char *word) {
    Node *n = NULL;
    if (bf_probe(text->filter, word) && ((n = ht_lookup(text->words, word)) != NULL)) {
        return (double) n->count / text->count;
    }
    return 0.0;
}

bool text_contains(Text *text, char *word) {
    return text && bf_probe(text->filter, word) && ht_lookup(text->words, word);
}

void text_print(Text *text) {
    HashTableIterator *hti = hti_create(text->words);

    for (Node *n = ht_iter(hti); n; n = ht_iter(hti)) {
        printf("(\"%s\", %" PRIu32 ")\n", n->word, n->count);
    }

    hti_delete(&hti);
}
