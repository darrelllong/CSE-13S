#ifndef NIL
#define NIL (void *)0
#endif

#ifndef _HASHTABLE_DL
#define _HASHTABLE_DL
#include "bloom.h"
#include "../Double/dll.h"
#include <stdint.h>

typedef struct hashTable hashTable;

struct hashTable {
  uint64_t hashKey[2]; // Salt
  uint32_t length;     // Length
  listHead **table;    // Array of pointers to linked lists
  bloom *b;            // Bloom filter
};

hashTable *newHT(uint32_t, uint32_t);

void delHT(hashTable *);

listNode *findHT(hashTable *, char *);

void insertHT(hashTable *, char *);

void printHT(hashTable *);

#endif
