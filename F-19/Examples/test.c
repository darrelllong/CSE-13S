#include <stdio.h>

#include "itor.h"
#include "itoa.h"

int main(void) {

  for (int i = -100; i < 100; i++) {
    printf("%d = %s = %s\n", i, itor(i), itoa(i));
  }
  return 0;
}
