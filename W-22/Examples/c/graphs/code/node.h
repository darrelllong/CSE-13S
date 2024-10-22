#pragma once

#include <stdint.h>

typedef struct Node Node;

struct Node {
    uint32_t v;
    Node *next;
    Node *prev;
};

Node *node_create(uint32_t v);

void node_delete(Node **n);
