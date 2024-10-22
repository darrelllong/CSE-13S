#include "hashTable.h"
#include <stdio.h>

#define HT_SIZE (1 << 12)
#define BF_SIZE (1 << 18)

int main(void) {
  hashTable *h = newHT(HT_SIZE, BF_SIZE);

  FILE *fp = fopen("./words", "r");
  char s[1 << 10];

  while (fscanf(fp, "%s", s) != EOF) {
    if (h) {
      insertHT(h, s);
    }
  }
  fclose(fp);

  while (scanf("%s", s) != EOF) {
    if (probeBF(h->b, s) && !findHT(h, s)) {
      printf("%s — false positive\n", s);
    } else if (findHT(h, s)) {
      printf("%s — found\n", s);
    } else {
      printf("%s — not found\n", s);
    }
  }
  for (uint32_t i = 0; i < 10; i += 1) {
    printf("\nh[%d] = ", i);
    printList(h->table[i]);
  }

  delHT(h);
  return 0;
}
