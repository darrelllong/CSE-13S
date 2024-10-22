#include "node.h"
#include <stdlib.h>

Node *node_create(uint32_t v) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->v = v;
    n->next = n->prev = NULL;
    return n;
}

void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}
