#include <stdio.h>

#include "dll.h"

int main(void) {
  listHead *h = newList();

  FILE *fp = fopen("./words", "r");
  char s[1 << 10];

  while (fscanf(fp, "%s", s) != EOF) {
    if (h) {
      (void) insertList(h, newNode(s));
    }
  }
  fclose(fp);

  if (h) {
    while (scanf("%s", s) != EOF) {
      listNode *n = newNode(s);
      if (n) {
        if (findMTF(h, n->key)) printf("%s - found\n", s);;
        delNode(n);
      }
    }

    // printList(h);
    destroyList(h);
    free(h);
  }
  return 0;
}
