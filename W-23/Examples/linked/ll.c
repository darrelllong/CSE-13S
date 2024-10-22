#include "ll.h"
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
    struct Node *prev;
};

static struct Node *node_new(int data) {
    struct Node *n = malloc(sizeof(struct Node));
    n->data = data;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

static void node_drop(struct Node *n) {
    free(n);
}

struct LinkedList ll_new(void) {
    struct LinkedList ll = {
        .head = node_new(0),
        .tail = node_new(0),
        .len = 0,
    };
    ll.head->next = ll.tail;
    ll.tail->prev = ll.head;
    return ll;
}

void ll_drop(struct LinkedList ll) {
    struct Node *curr = ll.head;
    while (curr != NULL) {
        struct Node *next = curr->next;
        node_drop(curr);
        curr = next;
    }
}

size_t ll_len(struct LinkedList *ll) {
    return ll->len;
}

void ll_add_front(struct LinkedList *ll, int data) {
    struct Node *n = node_new(data);
    n->next = ll->head->next;
    n->prev = ll->head;
    ll->head->next->prev = n;
    ll->head->next = n;
    ll->len += 1;
}

void ll_add_back(struct LinkedList *ll, int data) {
    struct Node *n = node_new(data);
    n->next = ll->tail;
    n->prev = ll->tail->prev;
    ll->tail->prev->next = n;
    ll->tail->prev = n;
    ll->len += 1;
}

bool ll_pop_front(struct LinkedList *ll, int *data) {
    if (ll_len(ll) == 0) {
        return false;
    }
    struct Node *n = ll->head->next;
    n->next->prev = ll->head;
    ll->head->next = n->next;
    ll->len -= 1;
    *data = n->data;
    node_drop(n);
    return true;
}

bool ll_pop_back(struct LinkedList *ll, int *data) {
    if (ll_len(ll) == 0) {
        return false;
    }
    struct Node *n = ll->tail->prev;
    n->prev->next = ll->tail;
    ll->tail->prev = n->prev;
    ll->len -= 1;
    *data = n->data;
    node_drop(n);
    return true;
}

void ll_print(struct LinkedList *ll) {
    printf("[");
    for (struct Node *curr = ll->head->next; curr != ll->tail; curr = curr->next) {
        printf("%d", curr->data);
        if (curr->next != ll->tail) {
            printf(", ");
        }
    }
    printf("]\n");
}
