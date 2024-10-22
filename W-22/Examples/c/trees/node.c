#include "node.h"
#include <stdio.h>
#include <stdlib.h>

Node *node_create(int key) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->key = key;
    n->left = n->right = n->parent = NULL;
    return n;
}

void node_print(Node *n) {
    printf("%d\n", n->key);
}

void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
}
