#include "ht.h"

#include "bst.h"
#include "speck.h"

#include <stdio.h>
#include <stdlib.h>

uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **bsts;
};

HashTable *ht_create(uint32_t size) {
    HashTable *h = (HashTable *)malloc(sizeof(HashTable));
    if (h) {
        // Leviathan
        h->salt[0] = 0x9846e4f157fe8840;
        h->salt[1] = 0xc5f318d7e055afb8;
        h->size = size;
        h->bsts = (Node **)calloc(size, sizeof(Node *));
        if (!h->bsts) {
            free(h);
            h = NULL;
        }
    }
    return h;
}

void ht_delete(HashTable **h) {
    for (uint32_t i = 0; i < (*h)->size; i += 1) {
        if ((*h)->bsts[i]) {
            bst_delete(&(*h)->bsts[i]);
        }
    }
    free((*h)->bsts);
    (*h)->bsts = NULL;
    free(*h);
    *h = NULL;
}

Node *ht_lookup(HashTable *h, char *oldspeak) {
    lookups += 1;
    uint32_t index = hash(h->salt, oldspeak) % h->size;
    return bst_find(h->bsts[index], oldspeak);
}

void ht_insert(HashTable *h, char *oldspeak, char *newspeak) {
    lookups += 1;
    uint32_t index = hash(h->salt, oldspeak) % h->size;
    h->bsts[index] = bst_insert(h->bsts[index], oldspeak, newspeak);
}

uint32_t ht_count(HashTable *h) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < h->size; i += 1) {
        count += h->bsts[i] ? 1 : 0;
    }
    return count;
}

double ht_avg_bst_size(HashTable *h) {
    uint32_t size = 0;
    for (uint32_t i = 0; i < h->size; i += 1) {
        size += bst_size(h->bsts[i]);
    }
    return (double)size / (double)ht_count(h);
}

double ht_avg_bst_height(HashTable *h) {
    uint32_t height = 0;
    for (uint32_t i = 0; i < h->size; i += 1) {
        height += bst_height(h->bsts[i]);
    }
    return (double)height / (double)ht_count(h);
}

void ht_print(HashTable *h) {
    for (uint32_t i = 0; i < h->size; i += 1) {
        if (h->bsts[i]) {
            bst_print(h->bsts[i]);
        }
    }
}
