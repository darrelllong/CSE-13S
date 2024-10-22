#include "bst.h"
#include <stddef.h>

Node *bst_create(void) {
    return NULL;
}

Node *bst_min(Node *root) {
    while (root && root->left) {
        root = root->left;
    }
    return root;
}

Node *bst_max(Node *root) {
    while (root && root->right) {
        root = root->right;
    }
    return root;
}

static int max(int x, int y) {
    return x > y ? x : y;
}

int bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

static int abs(int x) {
    return x < 0 ? -x : x;
}

bool bst_balanced(Node *root) {
    if (root) {
        if (abs(bst_height(root->left) - bst_height(root->right)) > 1) {
            return false;
        }
        return bst_balanced(root->left) && bst_balanced(root->right);
    }
    return true;
}

Node *bst_find(Node *root, int key) {
    if (root) {
        if (root->key > key) {
            return bst_find(root->left, key);
        } else if (root->key < key) {
            return bst_find(root->right, key);
        }
    }
    return root;
}

Node *bst_insert(Node *root, int key) {
    if (root) {
        if (root->key > key) {
            root->left = bst_insert(root->left, key);
        } else {
            root->right = bst_insert(root->right, key);
        }
        return root;
    }
    return node_create(key);
}

Node *bst_remove(Node *root, int key) {
    if (root) {
        if (root->key > key) {              // Remove from left subtree.
            root->left = bst_remove(root->left, key);
        } else if (root->key < key) {       // Remove from right subtree.
            root->right = bst_remove(root->right, key);
        } else {                            // Found the node.
            if (!root->left) {              // Node missing left child.
                Node *temp = root->right;
                node_delete(&root);
                root = temp;
            } else if (!root->right) {      // Node missing right child.
                Node *temp = root->left;
                node_delete(&root);
                root = temp;
            } else {                        // Node has two children.
                Node *temp = bst_min(root->right);
                root->key = temp->key;
                root->right = bst_remove(root->right, root->key);
            }
        }
    }
    return root;
}

void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}
