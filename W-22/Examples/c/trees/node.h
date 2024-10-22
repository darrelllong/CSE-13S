#pragma once

typedef struct Node Node;

struct Node {
    int key;
    Node *left;
    Node *right;
};

Node *node_create(int key);

void node_print(Node *n);

void node_delete(Node **n);
