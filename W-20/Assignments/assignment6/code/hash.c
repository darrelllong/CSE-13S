#include "gs.h"
#include "hash.h"
#include "ll.h"
#include "speck.h"
#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct HashTable {
  uint64_t salt[2];
  uint32_t length;
  ListNode **heads;
};

HashTable *ht_create(uint32_t length) {
  HashTable *h = (HashTable *)malloc(sizeof(HashTable));
  check(h, "HashTable not allocated!\n");

  h->length = length;
  h->heads = (ListNode **)calloc(h->length, sizeof(ListNode *));
  check(h->heads, "HashTable heads not allocated!\n");

  h->salt[0] = 0xFC28CA6885711CF7;
  h->salt[1] = 0x2841AF568222F773;
  return h;
}

void ht_delete(HashTable *h) {
  for (uint32_t i = 0; i < h->length; i += 1) {
    if (h->heads[i]) {
      ll_delete(h->heads[i]);
    }
  }

  free(h->heads);
  h->heads = NULL;
  free(h);
  h = NULL;
  return;
}

ListNode *ht_lookup(HashTable *h, char *key) {
  return ll_lookup(&h->heads[hash(h->salt, key) % h->length], key);
}

void ht_insert(HashTable *h, GoodSpeak *gs) {
  uint32_t index = hash(h->salt, gs_oldspeak(gs)) % h->length;
  h->heads[index] = ll_insert(&h->heads[index], gs);
  return;
}

uint32_t ht_count(HashTable *h) {
  uint32_t count = 0;

  for (uint32_t i = 0; i < h->length; i += 1) {
    if (h->heads[i]) {
      count += 1;
    }
  }

  return count;
}

void ht_print(HashTable *h) {
  for (uint32_t i = 0; i < h->length; i += 1) {
    if (h->heads[i]) {
      ll_print(h->heads[i]);
    }
  }

  return;
}
