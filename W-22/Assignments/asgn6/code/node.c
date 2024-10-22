#include "node.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n) {
        n->symbol = symbol;
        n->frequency = frequency;
        n->left = n->right = NULL;
    }
    return n;
}

void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

bool node_cmp(Node *n, Node *m) {
    return n->frequency > m->frequency;
}

Node *node_join(Node *left, Node *right) {
    Node *parent = node_create('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    return parent;
}

void node_print(Node *n) {
    if (iscntrl(n->symbol) || !isprint(n->symbol)) {
        printf("(0x%02" PRIx8 ", %" PRIu64 ")", n->symbol, n->frequency);
    } else {
        printf("('%c', %" PRIu64 ")", (char)n->symbol, n->frequency);
    }
}

void node_print_sym(Node *n) {
    if (iscntrl(n->symbol) || !isprint(n->symbol)) {
        printf("0x%02" PRIx8 "\n", n->symbol);
    } else {
        printf("'%c'\n", (char)n->symbol);
    }
}
