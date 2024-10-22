#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define strdup(s) (strcpy(malloc(strlen(s) + 1), s))

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->oldspeak = oldspeak ? strdup(oldspeak) : NULL;
    n->newspeak = newspeak ? strdup(newspeak) : NULL;
    n->next = n->prev = NULL;
    return n;
}

void node_delete(Node **n) {
    free((*n)->oldspeak);
    (*n)->oldspeak = NULL;
    free((*n)->newspeak);
    (*n)->newspeak = NULL;
    free(*n);
    *n = NULL;
}

void node_print(Node *n) {
    if (n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else {
        printf("%s\n", n->oldspeak);
    }
}
