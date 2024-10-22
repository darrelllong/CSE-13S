#include "ht.h"
#include "salts.h"
#include "speck.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **buckets;
};

struct HashTableIterator {
    HashTable *table;
    uint32_t bucket;
};

HashTable *ht_create(uint32_t size) {
    HashTable *h = (HashTable *)malloc(sizeof(HashTable));
    if (h) {
        h->salt[0] = SALT_HASHTABLE_LO;
        h->salt[1] = SALT_HASHTABLE_HI;
        h->size = size;
        h->buckets = (Node **)calloc(size, sizeof(Node *));
        if (!h->buckets) {
            free(h);
            h = NULL;
        }
    }
    return h;
}

void ht_delete(HashTable **h) {
    for (uint32_t i = 0; i < (*h)->size; i += 1) {
        if ((*h)->buckets[i]) {
            node_delete(&(*h)->buckets[i]);
        }
    }
    free((*h)->buckets);
    (*h)->buckets = NULL;
    free(*h);
    *h = NULL;
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

Node *ht_lookup(HashTable *h, char *word) {
    uint32_t count = 0;
    uint32_t index = hash(h->salt, word) % h->size;

    while (count < h->size) {
        Node *entry = h->buckets[index];
        if (entry && strcmp(entry->word, word) == 0) {
            return entry;
        }
        else if (!entry) { return NULL; }
        index = (index + 1) % h->size;
        count += 1;
    }

    return NULL;
}

Node *ht_insert(HashTable *h, char *word) {
    uint32_t count = 0;
    uint32_t index = hash(h->salt, word) % h->size;

    while (count < h->size) {
        Node *entry = h->buckets[index];
        if (entry && strcmp(word, entry->word) == 0) {
            entry->count += 1;
            return entry;
        } else if (!entry) {
            h->buckets[index] = node_create(word);
            return h->buckets[index];
        }
        index = (index + 1) % h->size;
        count += 1;
    }

    return NULL;
}

void ht_print(HashTable *h) {
    for (uint32_t i = 0; i < h->size; i += 1) {
        if (h->buckets[i]) {
            printf("[%" PRIu32 "] = ", i);
            node_print(h->buckets[i]);
        }
    }
}

HashTableIterator *hti_create(HashTable *ht) {
    HashTableIterator *hti = malloc(sizeof(HashTableIterator));
    hti->table = ht;
    hti->bucket = 0;
    return hti;
}

void hti_delete(HashTableIterator **hti) {
    free(*hti);
    *hti = NULL;
}

Node *ht_iter(HashTableIterator *hti) {
    Node *n = NULL;

    while (!n && hti->bucket < hti->table->size) {
        n = hti->table->buckets[hti->bucket++];
    }

    return n;
}
