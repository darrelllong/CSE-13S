#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "hash.h"
#include "hashTable.h"

static uint32_t hash(hashTable *h, const char *key) {
  union {
    uint64_t full;
    uint32_t half[2];
  } value;
  value.full = keyedHash(key, strlen(key), h->hashKey);
  return value.half[0] ^ value.half[1];
}

hashTable *newHT(const uint32_t k, const uint32_t bf) {
  hashTable *t = (hashTable *) malloc(sizeof(hashTable));
  if (t) {
    t->table      = (listNode **) calloc(k, sizeof(listNode *));
    t->length     = k;
    t->hashKey[0] = 0xb1a84340c5fda1e5; // Dr. Samuel Johnson
    t->hashKey[1] = 0x4b7049f8cb1b738a;
    t->b          = newBF(bf);
    return t;
  } else {
    return NIL;
  }
}

void delHT(hashTable *t) {
  for (uint32_t i = 0; i < t->length; i += 1) {
    delLL(t->table[i]); // Free each list
  }
  destroyBF(t->b); // Free the Bloom filter
  free(t->table); // Free the table
  free(t); // Free the container
  return;
}

listNode *findHT(hashTable *h, const char *key) {
  uint32_t t = hash(h, key) % h->length;
  if (probeBF(h->b, key)) { // Look only if it might be present
    return findLL(&h->table[t], key);
  } else {
    return NIL;
  }
}

void insertHT(hashTable *h, const char *key) {
  uint32_t t = hash(h, key) % h->length;
  insertBF(h->b, key);
  h->table[t] = insertLL(&h->table[t], key);
  return;
}

void printHT(const hashTable *h) {
  for (uint32_t i = 0; i < h->length; i += 1) {
    if (h->table[i] != NIL) {
      printf("h[%" PRIu32 "]: ", i);
      printLL(h->table[i]);
    }
  }
  return;
}
