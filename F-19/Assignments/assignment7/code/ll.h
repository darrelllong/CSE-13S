#ifndef __LL_H__
#define __LL_H__

#include "gs.h"
#include <stdbool.h>

extern bool move_to_front;

typedef struct ListNode ListNode;

ListNode *ll_node_create(GoodSpeak *gs);

void ll_node_delete(ListNode *n);

void ll_delete(ListNode *head);

GoodSpeak *ll_node_gs(ListNode *n);

ListNode *ll_insert(ListNode **head, GoodSpeak *gs);

ListNode *ll_lookup(ListNode **head, char *oldspeak);

void ll_node_print(ListNode *n);

void ll_print(ListNode *head);

#endif
