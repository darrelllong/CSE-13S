#include "trie.h"
#include <stdlib.h>

TrieNode *trie_node_create(Code c) {
    TrieNode *n = (TrieNode *)calloc(1, sizeof(TrieNode));
    n->code = c;
    return n;
}

void trie_node_delete(TrieNode **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
}

TrieNode *trie_create(void) {
    return trie_node_create(EMPTY_CODE);
}

void trie_reset(TrieNode *root) {
    for (int i = 0; i < ALPHABET; i += 1) {
        if (root->children[i]) {
            trie_delete(&root->children[i]);
        }
    }
}

void trie_delete(TrieNode **n) {
    if (*n) {
        for (int i = 0; i < ALPHABET; i += 1) {
            trie_delete(&(*n)->children[i]);
        }
        trie_node_delete(n);
    }
}

TrieNode *trie_step(TrieNode *n, Symbol s) {
    return n->children[s];
}
