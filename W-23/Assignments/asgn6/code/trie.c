#include "trie.h"
#include "code.h"
#include <stdlib.h>

TrieNode *trie_node_create(uint16_t code) {
    TrieNode *n = (TrieNode *)calloc(1, sizeof(TrieNode));
    n->code = code;
    return n;
}

void trie_node_delete(TrieNode *n) {
    free(n);
    n = NULL;
}

TrieNode *trie_create(void) {
    return trie_node_create(EMPTY_CODE);
}

void trie_reset(TrieNode *root) {
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
        if (root->children[i]) {
            trie_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

void trie_delete(TrieNode *n) {
    if (n) {
        for (uint16_t i = 0; i < ALPHABET; i += 1) {
            trie_delete(n->children[i]);
            n->children[i] = NULL;
        }
        trie_node_delete(n);
    }
}

TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
