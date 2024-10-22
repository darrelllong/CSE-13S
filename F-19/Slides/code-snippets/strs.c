// © 2019 DDEL

#include "strs.h"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Inline function to find max of two integers.
//
// a:   First integer to compare.
// b:   Second integer to compare.
//
static inline uint64_t max(uint64_t a, uint64_t b) {
  return (a > b) ? a : b;
}

//
// Constructor for a StrTreeNode.
//
// key:   The returned node's key.
//
StrTreeNode *str_tree_node_create(char *key) {
  StrTreeNode *node = (StrTreeNode *)malloc(sizeof(StrTreeNode));
  if (node) {
    node->left = NULL;
    node->right = NULL;
    node->entry = strndup(key, strlen(key));
    return node;
  }

  return NULL;
}

//
// Destructor for a StrTreeNode.
//
// node:  The StrTreeNode to free.
//
void str_tree_node_delete(StrTreeNode *node) {
  free(node->entry);
  node->entry = NULL;
  free(node);
  node = NULL;
  return;
}

//
// Frees a tree of StrTreeNodes.
//
// root:  The root of the tree to free.
//
void str_tree_delete(StrTreeNode *root) {
  if (!root) {
    return;
  }

  str_tree_delete(root->left);
  str_tree_delete(root->right);
  str_tree_node_delete(root);
  return;
}

//
// Finds a key in the tree.
// Returns a pointer if found and NULL otherwise.
//
// root:  The root of the tree to search in.
// key:   The key to find.
//
StrTreeNode *str_tree_find(StrTreeNode *root, char *key) {
  int direction = 0;
  StrTreeNode *chase = root;

  while (chase) {
    uint64_t max_len = max(strlen(key), strlen(chase->entry));
    direction = strncmp(key, chase->entry, max_len);

    if (direction < 0) {
      chase = chase->left;
    } else if (direction > 0) {
      chase = chase->right;
    } else {
      return chase;
    }
  }

  return NULL;
}

//
// Inserts a key into the tree.
// A key that already exists in the tree is not duplicated.
// An pointer to the entry is returned.
//
// root:  The root of the tree to insert into.
// key:   The key to insert.
//
StrTreeNode *str_tree_insert(StrTreeNode *root, char *key) {
  int direction = 0;
  StrTreeNode *lead = NULL;
  StrTreeNode *follow = NULL;

  if (!root) {
    return str_tree_node_create(key);
  } else {
    lead = root;
    follow = root;

    while (lead) {
      uint64_t max_len = max(strlen(key), strlen(lead->entry));
      direction = strncmp(key, lead->entry, max_len);

      if (direction < 0) {
        follow = lead;
        lead = lead->left;
      } else if (direction > 0) {
        follow = lead;
        lead = lead->right;
      } else {
        return lead;
      }
    }

    if (direction < 0) {
      follow->left = str_tree_node_create(key);
      return follow->left;
    } else if (direction > 0) {
      follow->right = str_tree_node_create(key);
      return follow->right;
    } else {
      return NULL;
    }
  }
}

//
// Debug function to print out a string tree.
//
// root:  The root of the tree to print.
// depth: The depth of the node to print.
//
void str_tree_print(StrTreeNode *root, int depth) {
  if (!root) {
    return;
  }

  for (int i = 0; i < depth; ++i) {
    printf("|   ");
  }

  printf("\"%s\"\n", root->entry);

  str_tree_print(root->left, depth + 1);
  str_tree_print(root->right, depth + 1);
  return;
}

int main(void) {
  StrTreeNode *root = str_tree_node_create("root");
  str_tree_insert(root, "hello");
  str_tree_insert(root, "hellos");
  str_tree_insert(root, "bye");
  str_tree_insert(root, "byes");
  str_tree_print(root, 0);

  StrTreeNode *find = str_tree_find(root, "hellos");
  assert(find);
  printf("\"%s\"\n", find->entry);

  str_tree_delete(root);
  return 0;
}
