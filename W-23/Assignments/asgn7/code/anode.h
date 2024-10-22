#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct AuthorNode AuthorNode;

struct AuthorNode {
    char *author;
    double dist;
};

AuthorNode *anode_create(char *author, double dist);

void anode_delete(AuthorNode **n);

bool anode_cmp(AuthorNode *n, AuthorNode *m);

void anode_print(AuthorNode *n);
