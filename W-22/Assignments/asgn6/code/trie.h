#pragma once

#include "defines.h"

typedef struct TrieNode TrieNode;

struct TrieNode {
    TrieNode *children[ALPHABET];
    Code code;
};

TrieNode *trie_node_create(Code c);

void trie_node_delete(TrieNode **n);

TrieNode *trie_create(void);

void trie_reset(TrieNode *root);

void trie_delete(TrieNode **n);

TrieNode *trie_step(TrieNode *n, Symbol s);
