#include "anode.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

AuthorNode *anode_create(uint32_t index, double dist) {
    AuthorNode *n = (AuthorNode *)malloc(sizeof(AuthorNode));
    if (n) {
        n->index = index;
        n->dist = dist;
    }
    return n;
}

void anode_delete(AuthorNode **n) {
    free(*n);
    *n = NULL;
}

bool anode_cmp(AuthorNode *n, AuthorNode *m) {
    return n->dist > m->dist;
}

void anode_print(AuthorNode *n) {
    printf("(index=%" PRIu32 ", dist=%lf)\n", n->index, n->dist);
}
