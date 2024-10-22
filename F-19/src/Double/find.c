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

    printList(h);

  listNode *p = findMTF(h, "dog");
  if (p != NIL) { printf("%s - found\n", p->key); }

  p = findMTF(h, "abasing");
  if (p != NIL) { printf("%s - found\n", p->key); }

    destroyList(h);
    free(h);
  }
  return 0;
}
