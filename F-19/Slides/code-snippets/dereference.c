#include <stdio.h>

void increment_two_ints(int *a, int *b) {
  (*a) += 1;
  (*b) += 1;
  return;
}

int main(void) {
  int x = 3;
  int y = 1;
  increment_two_ints(&x, &y);

  // The values of x and y are now 4 and 2 respectively.
  printf("The value of x is now: %d\n", x);
  printf("The value of y is now: %d\n", y);
  return 0;
}


