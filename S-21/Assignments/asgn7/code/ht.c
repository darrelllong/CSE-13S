#include "ht.h"

#include "ll.h"
#include "speck.h"

#include <stdio.h>
#include <stdlib.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *h = (HashTable *)malloc(sizeof(HashTable));
    if (h) {
        // Leviathan
        h->salt[0] = 0x9846e4f157fe8840;
        h->salt[1] = 0xc5f318d7e055afb8;
        h->size = size;
        h->mtf = mtf;
        h->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
        if (!h->lists) {
            free(h);
            h = NULL;
        }
    }
    return h;
}

void ht_delete(HashTable **h) {
    for (uint32_t i = 0; i < (*h)->size; i += 1) {
        if ((*h)->lists[i]) {
            ll_delete(&(*h)->lists[i]);
        }
    }
    free((*h)->lists);
    (*h)->lists = NULL;
    free(*h);
    *h = NULL;
}

Node *ht_lookup(HashTable *h, char *oldspeak) {
    uint32_t index = hash(h->salt, oldspeak) % h->size;
    if (!h->lists[index]) {
        return NULL;
    }
    return ll_lookup(h->lists[hash(h->salt, oldspeak) % h->size], oldspeak);
}

void ht_insert(HashTable *h, char *oldspeak, char *newspeak) {
    uint32_t index = hash(h->salt, oldspeak) % h->size;
    if (!h->lists[index]) {
        h->lists[index] = ll_create(h->mtf);
    }
    ll_insert(h->lists[index], oldspeak, newspeak);
}

uint32_t ht_count(HashTable *h) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < h->size; i += 1) {
        count += h->lists[i] ? 1 : 0;
    }
    return count;
}

void ht_print(HashTable *h) {
    for (uint32_t i = 0; i < h->size; i += 1) {
        if (h->lists[i]) {
            ll_print(h->lists[i]);
        }
    }
}
