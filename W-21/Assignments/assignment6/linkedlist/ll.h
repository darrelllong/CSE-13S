#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include "node.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct LinkedList LinkedList;

LinkedList *ll_create(bool mtf);

void ll_delete(LinkedList **l);

uint32_t ll_length(LinkedList *l);

void ll_insert(LinkedList *l, char *oldspeak, char *newspeak);

Node *ll_lookup(LinkedList *l, char *oldspeak);

void ll_print(LinkedList *l);

#endif
