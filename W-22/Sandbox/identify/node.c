#include "node.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_create(char *word) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->word = word ? strdup(word) : NULL;
    n->count = 1;
    n->next = n->prev = NULL;
    return n;
}

void node_delete(Node **n) {
    free((*n)->word);
    (*n)->word = NULL;
    free(*n);
    *n = NULL;
}

void node_print(Node *n) {
    printf("(\"%s\", %" PRIu32 ")\n", n->word, n->count);
}
