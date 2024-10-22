#include "huffman.h"
#include "defines.h"
#include "printer.h"
#include "pq.h"
#include "stack.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(ALPHABET);
    Node *root = NULL, *left = NULL, *right = NULL;

    // Enqueue occurring symbols to a priority queue.
    for (int i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            enqueue(pq, node_create((uint8_t)i, hist[i]));
        }
    }

    // Construct the tree, with higher frequency nodes near the top.
    // Dequeue twice for the left and right children, join and enqueue parent.
    while (pq_size(pq) > 1) {
        dequeue(pq, &left);
        dequeue(pq, &right);
        enqueue(pq, node_join(left, right));
    }

    // The root is the last internal node in the queue.
    dequeue(pq, &root);
    pq_delete(&pq);
    return root;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    static Code c;
    static uint8_t bit = 0;

    if (root) {
        // If we're at a leaf, we have the leaf's symbol's code.
        // Else, push a 0 for the left branch, push a 1 for the right branch.
        if (!root->left && !root->right) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, &bit);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &bit);
        }
    }
}

Node *rebuild_tree(uint16_t tree_size, uint8_t tree[static tree_size]) {
    Stack *s   = stack_create(ALPHABET);
    Node *root = NULL, *left = NULL, *right = NULL;

    for (uint16_t i = 0; i < tree_size; i += 1) {
        // A 'L' means the next tree symbol is the leaf's symbol.
        // Else, we're on an internal node. We pop twice and join the nodes.
        if (tree[i] == 'L') {
            i += 1;
            stack_push(s, node_create(tree[i], 0));
        } else {
            stack_pop(s, &right);
            stack_pop(s, &left);
            stack_push(s, node_join(left, right));
        }
    }

    // The root is the last internal node in the stack.
    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}

void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        node_delete(root);
    }
}
