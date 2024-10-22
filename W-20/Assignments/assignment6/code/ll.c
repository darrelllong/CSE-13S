#include "gs.h"
#include "ll.h"
#include "util.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool move_to_front = false;
uint64_t seeks = 0;
uint64_t links = 0;

struct ListNode {
  GoodSpeak *gs;
  ListNode *next;
};

ListNode *ll_node_create(GoodSpeak *gs) {
  ListNode *n = (ListNode *)malloc(sizeof(ListNode));
  check(n, "ListNode not allocated!\n");

  n->gs = gs;
  n->next = NULL;
  return n;
}

void ll_node_delete(ListNode *n) {
  gs_delete(n->gs);
  free(n);
  n = NULL;
  return;
}

void ll_delete(ListNode *head) {
  ListNode *temp = NULL;

  while (head) {
    temp = head;
    head = head->next;
    ll_node_delete(temp);
  }

  return;
}

GoodSpeak *ll_node_gs(ListNode *n) {
  return n->gs;
}

ListNode *ll_insert(ListNode **head, GoodSpeak *gs) {
  if (ll_lookup(head, gs_oldspeak(gs))) {
    gs_delete(gs);
    return *head;
  }

  ListNode *n = ll_node_create(gs);
  n->next = *head;
  return n;
}

ListNode *ll_lookup(ListNode **head, char *oldspeak) {
  seeks += 1;
  ListNode *prev = NULL;
  ListNode *target = NULL;

  for (ListNode *curr = *head; curr; curr = curr->next) {
    links += 1;
    uint64_t max_strlen = max(strlen(oldspeak), strlen(gs_oldspeak(curr->gs)));

    if (strncmp(oldspeak, gs_oldspeak(curr->gs), max_strlen) == 0) {
      if (move_to_front && prev) {
        prev->next = curr->next;
        curr->next = *head;
        *head = curr;
      }

      target = curr;
      break;
    }

    prev = curr;
  }

  return target;
}

void ll_node_print(ListNode *n) {
  printf("%s", gs_oldspeak(n->gs));

  if (gs_newspeak(n->gs)) {
    printf(" -> %s", gs_newspeak(n->gs));
  }

  printf("\n");
  return;
}

void ll_print(ListNode *head) {
  for (; head; head = head->next) {
    ll_node_print(head);
  }

  return;
}
