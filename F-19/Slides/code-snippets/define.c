#include <stdio.h>

#define PI 3.1415926

float circumference(float radius) {
  // The C preprocessor replaces PI with 3.1415926.
  return 2 * PI * radius;
}

int main(void) {
  float rad = 3.0;
  float cir = circumference(rad);
  printf("The circumference of a circle with radius %f is: %f\n", rad, cir);
  return 0;
}



