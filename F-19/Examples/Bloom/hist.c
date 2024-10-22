#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "hash.h"

#define M 901

uint64_t hashKey[] = { 0xfc28ca6885711cf7, // U.S. Consitution
                       0x2841af568222f773 };

uint64_t h(char *k) { return keyedHash(k, strlen(k), hashKey) % M; }

int main(void) {
  char s[1 << 16];
  uint32_t table[M];

  while (scanf("%s", s) != EOF) {
    table[h(s)] += 1;
  }

  for (int i = 0; i < M; i += 1) {
    printf("h[%4d]: ", i);
    for (int j = 0; j < table[i]; j += 1) {
      putchar('|');
    }
    putchar('\n');
  }

  return 0;
}
