#include <stdio.h>

void increment(int *a) {
  (*a) += 1;
  return;
}

int main(void) {
  // Make func_ptr point at increment().
  void (*func_ptr)(int *) = increment;

  int x = 5;

  // Increment x by invoking increment().
  func_ptr(&x);

  // The value of x is now: 6
  printf("The value of x is now: %d\n", x);
  return 0;
}


