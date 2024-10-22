#ifndef NIL
#define NIL (void *)0
#endif

#ifndef _HASHTABLE_DL
#define _HASHTABLE_DL
#include "bloom.h"
#include "ll.h"
#include <stdint.h>

typedef struct hashTable hashTable;

struct hashTable {
  uint64_t hashKey[2]; // Salt
  uint32_t length;     // Length
  listNode **table;    // Array of pointers to linked lists
  bloom *b;            // Bloom filter
};

hashTable *newHT(uint32_t, uint32_t);

void delHT(hashTable *);

listNode *findHT(hashTable *, const char *);

void insertHT(hashTable *, const char *);

void printHT(const hashTable *);

#endif
