#include "sets.h"
#include <inttypes.h>
#include <stdio.h>

enum items {
  A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U,
      V, W, X, Y, Z };

void printSet(uint32_t s) {
  printf("{ ");
  for (enum items i = A; i <= Z; i += 1) {
    if (memberSet(i, s)) {
      printf("%c ", (char)i + 'A');
    }
  }
  printf("}");
  return;
}

#define nl() printf("\n")

enum birds {EAGLE, BUZZARD, SPARROW, CROW, CHICKEN, TURKEY, DUCK};

enum mammals {DOG, WOLF, CAT, TIGER, RAT, SQUIRREL, DEER, ELK, MOOSE, COW};

int main(void) {

set vowels = (1<<A)|(1<<E)|(1<<I)|(1<<O)|(1<<U);

printSet(vowels); nl();

}
