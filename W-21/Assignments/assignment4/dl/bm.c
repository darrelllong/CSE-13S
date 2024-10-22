#include "bm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BYTE 8

struct BitMatrix {
  uint32_t r, c;
  uint8_t *b;
};

BitMatrix *bm_create(uint32_t r, uint32_t c) {
  BitMatrix *p = malloc(sizeof(BitMatrix));
  if (p) {
    p->r = r;
    p->c = c;
    p->b = calloc((r * c) / BYTE + ((r * c) % BYTE ? 1 : 0), sizeof(uint8_t));
    if (p->b) {
      return p;
    }
  }
  return NULL;
}

uint32_t bm_rows(BitMatrix *m) { return m->r; }

uint32_t bm_cols(BitMatrix *m) { return m->c; }

// Delete a bit matrix: it can either succeed or fail. It will fail if either of
// the pointers is NULL.

void bm_delete(BitMatrix *d) {
  if (d && d->b) {
    free(d->b);
    free(d);
  }
  return;
}

inline void bm_set_bit(BitMatrix *b, uint32_t r, uint32_t c) {
  const uint32_t k = (r * b->c) + c;
  b->b[k / BYTE] |= (0x1 << k % BYTE);
  return;
}

inline void bm_clr_bit(BitMatrix *b, uint32_t r, uint32_t c) {
  const uint32_t k = (r * b->c) + c;
  b->b[k / BYTE] &= ~(0x1 << k % BYTE);
  return;
}

inline uint8_t bm_get_bit(BitMatrix *b, uint32_t r, uint32_t c) {
  const uint32_t k = (r * b->c) + c;
  return (b->b[k / BYTE] >> k % BYTE) & 0x1;
}

bool bm_transpose(BitMatrix *a, BitMatrix *b) {
  if (a->r == b->c && a->c == b->r) {
    for (int i = 0; i < a->r; i += 1) {
      for (int j = 0; j < a->c; j += 1) {
        bm_get_bit(a, i, j) ? bm_set_bit(b, j, i) : bm_clr_bit(b, j, i);
      }
    }
    return true;
  }
  return false;
}

bool bm_multiply(BitMatrix *a, BitMatrix *b, BitMatrix *c) {
  if (a->r == b->c && a->c == b->r && c->r == b->r && c->c == a->c) {
    for (int i = 0; i < a->r; i += 1) {
      for (int j = 0; j < b->c; j += 1) {
        uint8_t sum = 0;
        for (int k = 0; k < a->c; k += 1) {
          sum ^= bm_get_bit(a, i, k) & bm_get_bit(b, k, j);
        }
        sum ? bm_set_bit(c, i, j) : bm_clr_bit(c, i, j);
      }
    }
    return true;
  }
  return false;
}

void bm_print(BitMatrix *p) {
  for (int i = 0; i < p->r; i += 1) {
    for (int j = 0; j < p->c; j += 1) {
      printf("%c ", '0' + bm_get_bit(p, i, j));
    }
    printf("\n");
  }
  return;
}

#define HEADER "-------------------\n"

#define BIG 20
#define SML 15
int main(void) {
  BitMatrix *m = bm_create(SML, BIG);
  BitMatrix *n = bm_create(SML, BIG);
  BitMatrix *o = bm_create(BIG, SML);
  BitMatrix *q = bm_create(BIG, BIG);
  for (int i = 0; i < m->r; i += 1) {
    bm_set_bit(m, i, i);
  }
  for (int i = 0; i < m->r; i += 1) {
    for (int j = 0; j < m->c; j += 1) {
      if (i % 2 || j % 2) {
        bm_set_bit(n, i, j);
      }
    }
  }

  bm_transpose(n, o);
  printf("M " HEADER);
  bm_print(m);
  printf("N " HEADER);
  bm_print(n);
  printf("trans(N) " HEADER);
  bm_print(o);
  printf("M x trans(N) " HEADER);
  bm_multiply(m, o, q);
  bm_print(q);
  bm_delete(m);
  bm_delete(n);
  bm_delete(o);
  bm_delete(q);

  return 0;
}
