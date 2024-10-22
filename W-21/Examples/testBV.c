#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX 1000
int main(void) {
  BitVector *v = bv_create(MAX);
  for (int i = 0; i < MAX; i += 1) {
    if (i % 2) {
      bv_set_bit(v, i);
    }
  }
  bv_print(v);
  for (int i = 0; i < MAX; i += 1) {
    if (!(i % 2)) {
      bv_set_bit(v, i);
    }
  }
  bv_print(v);
  for (int i = 0; i < MAX; i += 1) {
    if (i % 2) {
      bv_clr_bit(v, i);
    }
  }
  bv_print(v);
  for (int i = 0; i < MAX; i += 1) {
    if (!(i % 2)) {
      bv_clr_bit(v, i);
    }
  }
  bv_print(v);
  srandom(0xdeadd00d);
  for (int i = 0; i < 2 * MAX; i += 1) {
      bv_set_bit(v, random() % MAX);
  }
  bv_print(v);
  srandom(0xdeadd00d);
  for (int i = 0; i < 2 * MAX; i += 1) {
      bv_clr_bit(v, random() % MAX);
  }
  bv_print(v);
  bv_delete(v);
  return 0;
}
