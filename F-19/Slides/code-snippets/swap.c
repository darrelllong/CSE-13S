#include <stdio.h>

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
  return;
}

int main(void) {
  int x = 5;
  int y = 7;
  swap(&x, &y);

  printf("The value of x is: %d\n", x);
  printf("The value of y is: %d\n", y);

  // Output for program:
  // "The value of x is: 7"
  // "The value of y is: 5"
  return 0;
}


