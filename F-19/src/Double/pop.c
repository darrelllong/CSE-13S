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
    while ((p = popList(h)) != NIL) {
        printf("%s\n", p->key);
        delNode(p);
    }

    destroyList(h);
    free(h);
  }
  return 0;
}
