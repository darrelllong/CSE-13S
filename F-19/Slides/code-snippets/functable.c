#include <stdio.h>

enum { INCREMENT, DECREMENT };

void increment(int *a) {
  (*a) += 1;
  return;
}

void decrement(int *a) {
  (*a) -= 1;
  return;
}

int main(void) {
  // func_table is an array of function pointers.
  // func_table[0] is increment().
  // func_table[1] is decrement().
  void (*func_table[])(int *) = { increment, decrement };

  int x = 5;
  func_table[INCREMENT](&x);
  printf("The value of x is now: %d\n", x); // x = 6

  func_table[DECREMENT](&x);
  printf("The value of x is now: %d\n", x); // x = 5
  return 0;
}



