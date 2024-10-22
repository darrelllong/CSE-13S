// © 2019 DDEL

#ifndef __STRS_H__
#define __STRS_H__

typedef struct StrTreeNode StrTreeNode;

//
// Struct definition of a StrTreeNode.
//
// entry:   The entry the node encapsulates.
// left:    The left StrTreeNode child.
// right:   The right StrTreeNode child.
//
struct StrTreeNode {
  char *entry;
  StrTreeNode *left;
  StrTreeNode *right;
};

//
// Constructor for a StrTreeNode.
//
// key:   The returned node's key.
//
StrTreeNode *str_tree_node_create(char *key);

//
// Destructor for a StrTreeNode.
//
// node:  The StrTreeNode to free.
//
void str_tree_node_delete(StrTreeNode *node);

//
// Frees a tree of StrTreeNodes.
//
// root:  The root of the tree to free.
//
void str_tree_delete(StrTreeNode *root);

//
// Finds a key in the tree.
// Returns a pointer if found and NULL otherwise.
//
// root:  The root of the tree to search in.
// key:   The key to find.
//
StrTreeNode *str_tree_find(StrTreeNode *root, char *key);

//
// Inserts a key into the tree.
// A key that already exists in the tree is not duplicated.
// An pointer to the entry is returned.
//
// root:  The root of the tree to insert into.
// key:   The key to insert.
//
StrTreeNode *str_tree_insert(StrTreeNode *root, char *key);

//
// Debug function to print out a string tree.
//
// root:  The root of the tree to print.
// depth: The depth of the node to print.
//
void str_tree_print(StrTreeNode *root, int depth);

#endif
