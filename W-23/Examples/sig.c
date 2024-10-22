#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void croak() {
  puts("You got me!");
  exit(0);
}

void wound() {
  puts("Ouch!");
  signal(SIGINT, croak);
  return;
}

int main(void) {
  signal(SIGINT, wound);
  while (1)
    ;
  return 0;
}
