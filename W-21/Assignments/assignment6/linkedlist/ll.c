#include "ll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

LinkedList *ll_create(bool mtf) {
    LinkedList *l = (LinkedList *) malloc(sizeof(LinkedList));
    l->length = 0;
    l->mtf = mtf;
    l->head = node_create(NULL, NULL);
    l->tail = node_create(NULL, NULL);
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

void ll_insert(LinkedList *l, char *oldspeak, char *newspeak) {
    if (!ll_lookup(l, oldspeak)) {
        Node *n = node_create(oldspeak, newspeak);
        n->next = l->head->next;
        n->prev = l->head;
        l->head->next->prev = n;
        l->head->next = n;
        l->length += 1;
    }
}

Node *ll_lookup(LinkedList *l, char *oldspeak) {
    for (Node *n = l->head->next; n != l->tail; n = n->next) {
        if (strcmp(n->oldspeak, oldspeak) == 0) {
            if (l->mtf) {
                n->prev->next = n->next;
                n->next->prev = n->prev;
                n->next = l->head->next;
                n->prev = l->head;
                l->head->next->prev = n;
                l->head->next = n;
            }
            return n;
        }
    }
    return NULL;
}

void ll_print(LinkedList *l) {
    Node *n = l->head->next;
    for (uint32_t i = 0; i < l->length; i += 1) {
        printf("(%s -> %s)", n->oldspeak, n->newspeak ? n->newspeak : "null");
        if (i + 1 != l->length) {
            printf(" => ");
        }
        n = n->next;
    }
    printf("\n");
}
