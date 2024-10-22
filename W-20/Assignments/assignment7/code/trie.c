#include "code.h"
#include "trie.h"
#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Constructor for a TrieNode.
//
// code:    Code of the constructed TrieNode.
// returns: Pointer to a TrieNode that has been allocated memory.
//
TrieNode *trie_node_create(uint16_t code) {
  TrieNode *n = (TrieNode *) calloc(1, sizeof(TrieNode));
  check(n, "Failed to calloc() TrieNode.\n");

  n->code = code;
  return n;
}

//
// Destructor for a TrieNode.
//
// n:       TrieNode to free allocated memory for.
// returns: Void.
//
void trie_node_delete(TrieNode *n) {
  free(n);
  return;
}

//
// Initializes a Trie: a root TrieNode with the index EMPTY_CODE.
//
// returns: Pointer to the root of a Trie.
//
TrieNode *trie_create(void) {
  return trie_node_create(EMPTY_CODE);
}

//
// Resets a Trie to just the root TrieNode.
//
// root:    Root of the Trie to reset.
// returns: Void.
//
void trie_reset(TrieNode *root) {
  for (uint16_t i = 0; i < ALPHABET; i += 1) {
    if (root->children[i]) {
      trie_delete(root->children[i]);
      root->children[i] = NULL;
    }
  }

  return;
}

//
// Deletes a sub-Trie starting from the sub-Trie's root.
//
// n:       Root of the sub-Trie to delete.
// returns: Void.
//
void trie_delete(TrieNode *n) {
  if (n) {
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
      trie_delete(n->children[i]);
    }

    trie_node_delete(n);
    n = NULL;
  }

  return;
}

//
// Returns a pointer to the child TrieNode reprsenting the symbol sym.
// If the symbol doesn't exist, NULL is returned.
//
// n:       TrieNode to step from.
// sym:     Symbol to check for.
// returns: Pointer to the TrieNode representing the symbol.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
  return n->children[sym];
}
