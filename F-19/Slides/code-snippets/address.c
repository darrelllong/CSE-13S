#include <stdio.h>

int main(void) {
  int a = 42;
  int *ptr_a = &a;
  int *ptr_b = &a;

  // Two pointers can point to the same address.
  printf("The address of pointer A is: %p\n", ptr_a);
  printf("The address of pointer B is: %p\n", ptr_b);
  return 0;
}


