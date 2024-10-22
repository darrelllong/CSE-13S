#ifndef __TRIE_H__
#define __TRIE_H__

#include <inttypes.h>

#define ALPHABET 256

typedef struct TrieNode TrieNode;

//
// Struct definition for a TrieNode.
//
// children:  An array of TrieNode pointers.
// code:      The TrieNode's code number.
//
struct TrieNode {
  TrieNode *children[ALPHABET];
  uint16_t code;
};

//
// Constructor for a TrieNode.
//
// code:   The TrieNode's unique code.
//
TrieNode *trie_node_create(uint16_t code);

//
// Destructor for a TrieNode.
//
// n:  The TrieNode.
//
void trie_node_delete(TrieNode *n);

//
// Creates and initializes a new Trie.
// All ASCII characters are initially added as the root's children.
//
TrieNode *trie_create(void);

//
// Resets a Trie to having just the ASCII characters as the root's children.
//
// root:  Root of the Trie to reset.
//
void trie_reset(TrieNode *root);

//
// Destructor for a TrieNode.
//
// n:  The TrieNode.
//
void trie_delete(TrieNode *n);

//
// Returns the TrieNode one step down the Trie for the next symbol.
// Returns NULL if the symbol doesn't exist.
//
// n:     The TrieNode to step down from.
// sym:   The symbol to step down to.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym);

//
// Prints out a TrieNode.
//
// n:   TrieNode to print.
//
void trie_node_print(TrieNode *n);

//
// Wrapper function to recursively print out a Trie.
//
// root:  Root of the Trie to print.
//
void trie_print(TrieNode *n);

#endif
