#ifndef __NODE_H__
#define __NODE_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct Node Node;

struct Node {
    Node *left;
    Node *right;
    uint8_t symbol;
    uint64_t frequency;
};

Node *node_create(uint8_t symbol, uint64_t frequency);

void node_delete(Node **n);

Node *node_join(Node *left, Node *right);

void node_print(Node *n);

bool node_cmp(Node *n, Node *m);

void node_print_sym(Node *n);

#endif
