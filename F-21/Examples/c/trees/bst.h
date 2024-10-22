#pragma once

#include "node.h"
#include <stdbool.h>

Node *bst_create(void);

Node *bst_min(Node *root);

Node *bst_max(Node *root);

int bst_height(Node *root);

bool bst_balanced(Node *root);

Node *bst_find(Node *root, int key);

Node *bst_insert(Node *root, int key);

Node *bst_remove(Node *root, int key);

void bst_delete(Node **root);
