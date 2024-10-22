#include <stdio.h>

#include "dll.h"

int main(void) {
  listHead *h = newList();
  char s[1 << 10];

  if (h) {
    while (scanf("%s", s) != EOF) {
      listNode *n = newNode(s);
      if (n) {
        insertList(h, n);
      }
    }

    listNode *p;
    while ((p = dropList(h)) != NIL) {
        printf("%s\n", p->key);
        delNode(p);
    }
// fprintf(stderr, "destroylist\n"); fflush(stderr);
    destroyList(h);
  }
  return 0;
}
