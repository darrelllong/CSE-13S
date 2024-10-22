#include "io.h"
#include "trie.h"
#include "word.h"
#include <stddef.h>

#define IS_POW_2(x) (!(x & (x - 1)))

static int bitwidth(Code c) {
    static int width = 2;

    if (c == START_CODE) {
        width = 2;
    } else if (IS_POW_2(c)) {
        width += 1;
    }

    return width;
}

void encode(int infile, int outfile) {
    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    Symbol curr_sym = 0;
    Symbol prev_sym = 0;
    Code next_code = START_CODE;

    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);

        if (next_node) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bitwidth(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code++);
            curr_node = root;
        }

        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }

        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bitwidth(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }

    write_pair(outfile, STOP_CODE, 0, bitwidth(next_code));
    flush_pairs(outfile);
    trie_delete(&root);
}

void decode(int infile, int outfile) {
    WordTable *wt = wt_create();
    Symbol curr_sym = 0;
    Code curr_code = 0;
    Code next_code = START_CODE;

    while (read_pair(infile, &curr_code, &curr_sym, bitwidth(next_code))) {
        wt->words[next_code] = word_append_sym(wt->words[curr_code], curr_sym);
        write_word(outfile, wt->words[next_code++]);

        if (next_code == MAX_CODE) {
            wt_reset(wt);
            next_code = START_CODE;
        }
    }

    flush_words(outfile);
    wt_delete(&wt);
}
