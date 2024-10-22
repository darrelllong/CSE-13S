#ifndef __PRINTER_H__
#define __PRINTER_H__

#include "node.h"

// Assumes existence of n->left and n->right.
void binary_tree_print(Node *root, void (*print)(Node *n), int max_depth);

#endif
