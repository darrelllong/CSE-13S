#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "bloom.h"

#define BITS	1000000

#define check(b, s) { if (probeBF(b, s)) printf("%s - Maybe\n", s); \
else printf("%s - Nope\n", s); }

#define MIN(x,y) (x < y ? x : y)

void printBF(bloom *b) {
  if (b && b->f) {
    for (uint32_t i = 0; i < MIN(80, b->f->l); i += 1) {
      printf("%c", valBit(b->f, i) ? '1' : '0');
    }
    printf("\n");
    return;
  }
}

void printLoad(bloom *b) {
  uint32_t sum = 0;
  if (b && b->f) {
    for (uint32_t i = 0; i < b->f->l; i += 1) {
      sum += valBit(b->f, i) ? 1 : 0;
    }
    printf("Load %lf\n", 100.0 * (double)sum / b->f->l);
    return;
  }
}

int main(void) {
  char s[1 << 16];
  bloom *b = newBF(BITS);

  if (b) {
    printBF(b);
    while (scanf("%s", s) != EOF) {
      insertBF(b, s);
    }

    printBF(b);
    printLoad(b);

    check(b, "mother");
    check(b, "brother");
    check(b, "granada");
    check(b, "father");
    check(b, "tiajuana");
    check(b, "ziggy");
    check(b, "JASON");
    check(b, "Jason");
    check(b, "abracadabra");

    destroyBF(b);
  }
  return 0;
}
