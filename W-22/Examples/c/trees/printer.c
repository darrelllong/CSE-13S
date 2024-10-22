#include "printer.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

static Node *root = NULL;

typedef enum Branch { Left, Right } Branch;

static bool brancher(Node *n, Node *target, Branch branches[], int *top) {
    if (n) {
        if (n == target) {
            return true;
        } else {
            branches[(*top)++] = Left;
            if (brancher(n->left, target, branches, top)) {
                return true;
            }
            (*top) -= 1;

            branches[(*top)++] = Right;
            if (brancher(n->right, target, branches, top)) {
                return true;
            }
            (*top) -= 1;
        }
    }

    return false;
}

static int zag_depth(Node *n, int zag, int max_depth) {
    int top = 0, depth = 0;

    Branch branches[max_depth];
    brancher(root, n, branches, &top);

    Branch b = branches[0];

    for (int i = 1; i < top && zag; i += 1) {
        if (branches[i] != b) {
            zag -= 1;
            b = branches[i];
        }
        depth += 1;
    }

    return depth - 1;
}

static void printer(Node *n, int depth, int zags, Branch b, void (*print)(Node *), int max_depth) {
    if (n) {
        printer(n->right, depth + 1, b == Right ? zags : zags + 1, Right, print, max_depth);

        int branches = 1;
        for (int i = 0; i < depth; i++) {
            if (zags && branches != zags + 1 && i == zag_depth(n, branches, max_depth)) {
                printf(" │   ");
                branches += 1;
            } else {
                printf("     ");
            }
        }

        printf(b == Right ? " ┌── " : " └── ");
        print(n);
        printer(n->left, depth + 1, b == Right ? zags + 1 : zags, Left, print, max_depth);
    }
}

void binary_tree_print(Node *n, void (*print)(Node *)) {
    if ((root = n)) {
        printer(root->right, 0, 0, Right, print, 4096);
        print(root);
        printer(root->left, 0, 0, Left, print, 4096);
        fflush(stdout);
    }
}
