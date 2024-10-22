#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define BYTE 8

typedef struct bv {
  char *v;
  uint32_t l;
} bitVector;

void print_bv(bitVector *b) {
  for (int i = 0; i < b->l; i += 1) {
    uint32_t c = i / BYTE;
    uint32_t x = i % BYTE;
    putchar((b->v[c] & (0x1 << x)) ? '1' : '0');
  }
  putchar('\n');
  return;
}

int main(void) {
  bitVector b;
  b.v = "No one expects the Spanish Inquisition!";
  b.l = strlen(b.v) * BYTE;

  print_bv(&b);
  return 0;
}
