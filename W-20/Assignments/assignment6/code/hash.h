#ifndef __HASH_H__
#define __HASH_H__

#include "gs.h"
#include "ll.h"
#include <inttypes.h>

typedef struct HashTable HashTable;

HashTable *ht_create(uint32_t length);

void ht_delete(HashTable *h);

ListNode *ht_lookup(HashTable *h, char *key);

void ht_insert(HashTable *h, GoodSpeak *gs);

uint32_t ht_count(HashTable *h);

void ht_print(HashTable *h);

#endif
