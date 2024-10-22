#pragma once

#include <stddef.h>
#include <stdbool.h>

struct LinkedList {
    struct Node *head;
    struct Node *tail;
    size_t len;
};

struct LinkedList ll_new(void);

void ll_drop(struct LinkedList ll);

size_t ll_len(struct LinkedList *ll);

void ll_add_front(struct LinkedList *ll, int data);

void ll_add_back(struct LinkedList *ll, int data);

bool ll_pop_front(struct LinkedList *ll, int *data);

bool ll_pop_back(struct LinkedList *ll, int *data);

void ll_print(struct LinkedList *ll);
