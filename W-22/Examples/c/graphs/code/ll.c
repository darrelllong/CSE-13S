#include "ll.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

LinkedList *ll_create(void) {
    LinkedList *l = (LinkedList *) malloc(sizeof(LinkedList));
    l->length = 0;
    l->head = node_create(0);
    l->tail = node_create(0);
    l->head->next = l->tail;
    l->tail->prev = l->head;
    return l;
}

void ll_delete(LinkedList **l) {
    while ((*l)->head) {
        Node *t = (*l)->head->next;
        node_delete(&(*l)->head);
        (*l)->head = t;
    }
    free(*l);
    *l = NULL;
}

uint32_t ll_length(LinkedList *l) {
    return l->length;
}

void ll_insert(LinkedList *l, uint32_t v) {
    Node *n = node_create(v);
    n->next = l->head->next;
    n->prev = l->head;
    l->head->next->prev = n;
    l->head->next = n;
    l->length += 1;
}

Node *ll_lookup(LinkedList *l, uint32_t v) {
    for (Node *n = l->head->next; n != l->tail; n = n->next) {
        if (n->v == v) {
            n->prev->next = n->next;
            n->next->prev = n->prev;
            n->next = l->head->next;
            n->prev = l->head;
            l->head->next->prev = n;
            l->head->next = n;
            return n;
        }
    }
    return NULL;
}

void ll_print(LinkedList *l) {
    Node *n = l->head->next;
    for (uint32_t i = 0; i < l->length; i += 1) {
        printf("%" PRIu32, n->v);
        if (i + 1 != l->length) {
            printf(" -> ");
        }
        n = n->next;
    }
    printf("\n");
}
