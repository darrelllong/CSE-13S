#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Double/dll.h"
#include "hash.h"
#include "hashTable.h"

static uint32_t hash(hashTable *h, char *key) {
  union {
    uint64_t full;
    uint32_t half[2];
  } value;
  value.full = keyedHash(key, strlen(key), h->hashKey);
  return value.half[0] ^ value.half[1];
}

hashTable *newHT(uint32_t k, uint32_t bf) {
  hashTable *t = (hashTable *)malloc(sizeof(hashTable));
  if (t) {
    t->table = (listHead **)calloc(k, sizeof(listHead *));
    if (t->table) {
      for (uint32_t i = 0; i < k; i += 1) {
        t->table[i] = newList();
      }
    }
    t->length = k;
    t->hashKey[0] = 0xb1a84340c5fda1e5; // Dr. Samuel Johnson
    t->hashKey[1] = 0x4b7049f8cb1b738a;
    t->b = newBF(bf);
    return t;
  } else {
    return NIL;
  }
}

void delHT(hashTable *t) {
  for (uint32_t i = 0; i < t->length; i += 1) {
    destroyList(t->table[i]); // Free each list
    free(t->table[i]);
  }
  destroyBF(t->b); // Free the Bloom filter
  free(t->table); // Free the table
  free(t); // Free the container
  return;
}

listNode *findHT(hashTable *h, char *key) {
  uint32_t t = hash(h, key) % h->length;
  if (probeBF(h->b, key)) { // Look only if it might be present
    return findMTF(h->table[t], key);
  } else {
    return NIL;
  }
}

void insertHT(hashTable *h, char *key) {
  uint32_t t = hash(h, key) % h->length;
  listNode *p = newNode(key);
  insertBF(h->b, key);
  if (p) {
    //insertList(h->table[t], p);
    prependList(h->table[t], p);
  }
  return;
}

void printHT(hashTable *h) {
  for (uint32_t i = 0; i < h->length; i += 1) {
    if (h->table[i] != NIL) {
      printf("h[%" PRIu32 "]: ", i);
      printList(h->table[i]);
    }
  }
  return;
}
