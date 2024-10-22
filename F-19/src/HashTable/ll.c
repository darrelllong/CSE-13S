#include "ll.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool moveToFront = true;

void setMTF(bool v) { moveToFront = v; return; }

listNode *newNode(const char *key) {
  listNode *t = (listNode *) malloc(sizeof(listNode));
  if (t) {
    t->key  = strdup(key);
    t->next = NIL;
    return t;
  } else {
    return NIL;
  }
}

void delNode(listNode *n) { free(n->key); free(n); return; }

void delLL(listNode *n) {
  while (n != NIL) {
    listNode *t = n;
    n = n->next;
    delNode(t);
  }
  return;
}

void printLL(listNode *n) {
  for (listNode *t = n; t != NIL; t = t->next) {
    printf("%s\n", t->key);
  }
  return;
}

listNode *findLL(listNode **n, const char *key) {
  listNode *follow = NIL;
  for (listNode *t = *n; t != NIL; t = t->next) {
    if (strcmp(t->key, key) == 0) {
      if (moveToFront && follow != NIL) {
        follow->next = t->next;
        t->next = *n;
        *n = t;
      }
      return t;
    }
    follow = t;
  }
  return NIL;
}

listNode *insertLL(listNode **n, const char *key) {
  if (findLL(n, key)) {
    return *n;
  } else {
    listNode *t = newNode(key);
    if (t) {
      t->next = *n;
    }
    return t;
  }
}
