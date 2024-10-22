#include <stdint.h>
#include <stdio.h>

int main(void) {

  printf("Celsius Kelvin Fahrenheit Rankine  Delisle Reaumur   Romer\n");
  printf("------- ------ ---------- -------  ------- -------   -----\n");
  float i;
  for (i = 0; i < 200; i += 10) {
    printf("%7.2f", i); // Celsius
    printf("%7.2f", i + 273.15); // Kelvin
    printf("%11.2f", (i * 1.8) + 32); // Fahrenheit
    printf("%8.2f  ", (i + 273.15) * 1.8); // Rankine
    printf("%7.2f", (100 - i) * 1.5); // Delisle
    printf("%8.2f", i * 0.8); // Reaumur
    printf("%8.2f\n", (i * 0.525) + 7.5); // Romer
  }

  return 0;
}
