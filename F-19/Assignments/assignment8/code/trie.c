#include "trie.h"
#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Constructor for a TrieNode.
//
// children:  An array of TrieNode pointers.
// code:      The TrieNode's unique code.
//
TrieNode *trie_node_create(uint16_t code) {
  TrieNode *n = (TrieNode *)calloc(1, sizeof(TrieNode));
  ASSERT(n, "%s\n", "Failed to calloc() TrieNode.");

  n->code = code;
  return n;
}

//
// Destructor for a TrieNode.
//
// n:  The TrieNode.
//
void trie_node_delete(TrieNode *n) {
  free(n);
  return;
}

//
// Creates and initializes a new Trie.
// All ASCII characters are initially added as the root's children.
//
TrieNode *trie_create(void) {
  TrieNode *root = trie_node_create(0);

  for (uint16_t i = 0; i < ALPHABET; ++i) {
    root->children[i] = trie_node_create(i);
  }

  return root;
}

//
// Resets a Trie to having just the ASCII characters as the root's children.
//
// root:  Root of the Trie to reset.
//
void trie_reset(TrieNode *root) {
  for (uint32_t i = 0; i < ALPHABET; i += 1) {
    for (uint32_t j = 0; j < ALPHABET; j += 1) {
      if (root->children[i]->children[j]) {
        trie_delete(root->children[i]->children[j]);
        root->children[i]->children[j] = NULL;
      }
    }
  }

  return;
}

//
// Recursively frees memory allocated for an entire Trie.
//
// n:  The root of the sub-Trie to free (starts with actual root).
//
void trie_delete(TrieNode *n) {
  if (n) {
    for (uint16_t i = 0; i < ALPHABET; ++i) {
      trie_delete(n->children[i]);
    }

    trie_node_delete(n);
    n = NULL;
  }

  return;
}

//
// Returns the TrieNode one step down the Trie for the next symbol.
// Returns NULL if the symbol doesn't exist.
//
// n:     The TrieNode to step down from.
// sym:   The symbol to step down to.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
  return n->children[sym] ? n->children[sym] : NULL;
}

//
// Prints out relevant fields of a TrieNode.
//
// n:   TrieNode to print.
//
void trie_node_print(TrieNode *n) {
  printf("Code: %" PRIu16 "\n", n->code);
  return;
}

//
// Helper function to recursively print out a Trie.
//
// n:     TrieNode to start printing from.
// depth: The depth of the current node in the Trie.
//
static void trie_print_helper(TrieNode *n, int depth) {
  if (!depth) {
    printf("<<ROOT>>\n");
    depth += 1;
  }

  for (uint16_t i = 0; i < ALPHABET; i += 1) {
    if (n->children[i]) {
      for (int j = 0; j < depth; j += 1) {
        printf("|  ");
      }

      switch (i) {
      case 9:
        printf("Symbol: '\\t'  ");
        break;
      case 10:
        printf("Symbol: '\\n'  ");
        break;
      case 11:
        printf("Symbol: '\\v'  ");
        break;
      case 12:
        printf("Symbol: '\\f'  ");
        break;
      case 13:
        printf("Symbol: '\\r'  ");
        break;
      default:
        printf("Symbol: '%c'  ", (char)i);
      }

      trie_node_print(n->children[i]);
      trie_print_helper(n->children[i], depth + 1);
    }
  }

  return;
}

//
// Wrapper function to recursively print out a Trie.
//
// root:  Root of the Trie to print.
//
void trie_print(TrieNode *root) {
  trie_print_helper(root, 0);
  return;
}
