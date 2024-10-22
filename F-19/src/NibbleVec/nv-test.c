#include "nv.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  srand(time(NULL));

  NibbleVector *n = nv_create((rand() & 0xF) + 3);

  for (uint32_t i = 0; i < n->length; i += 1) {
    uint8_t nibble = rand() & 0x0F;
    printf("Setting 0x%0X at nibble %d\n", nibble, i);
    nv_set_nibble(n, i, nibble);
  }

  nv_print(n);

  for (uint32_t i = 0; i < n->length / 2; i += 1) {
    printf("Clearing 0x%0X at nibble %d\n", nv_get_nibble(n, i), i);
    nv_clr_nibble(n, i);
  }

  nv_print(n);

  nv_delete(n);
  return 0;
}
