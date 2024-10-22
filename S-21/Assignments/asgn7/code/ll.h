#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include "node.h"

#include <stdbool.h>
#include <stdint.h>

extern uint64_t seeks; // Number of seeks performed.
extern uint64_t links; // Number of links traversed.

typedef struct LinkedList LinkedList;

LinkedList *ll_create(bool mtf);

void ll_delete(LinkedList **ll);

uint32_t ll_length(LinkedList *ll);

Node *ll_lookup(LinkedList *ll, char *oldspeak);

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak);

void ll_print(LinkedList *ll);

#endif
