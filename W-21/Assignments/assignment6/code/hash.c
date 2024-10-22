#include "hash.h"
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
    HashTable *h = (HashTable *) malloc(sizeof(HashTable));
    if (h) {
        h->salt[0] = 0X85AE998311115AE3;
        h->salt[1] = 0XB6FAC2AE33A40089;
        h->size = size;
        h->mtf = mtf;
        h->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
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

void ht_print(HashTable *h) {
    for (uint32_t i = 0; i < h->size; i += 1) {
        if (h->lists[i]) {
            ll_print(h->lists[i]);
        }
    }
}
