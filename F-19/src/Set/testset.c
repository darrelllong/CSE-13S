#include "sets.h"
#include <inttypes.h>
#include <stdio.h>

enum items {
  A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N,
  O, P, Q, R, S, T, U, V, W, X, Y, Z }; 

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

int main(void) {
  uint32_t x = 0, y = 0;

  x = insertSet(A, x);
  x = insertSet(P, x);
  x = insertSet(Q, x);
  x = insertSet(W, x);
  x = insertSet(X, x);
  printf("x = ");
  printSet(x);
  nl();
  y = insertSet(B, y);
  y = insertSet(D, y);
  y = insertSet(W, y);
  y = insertSet(X, y);
  y = insertSet(Z, y);
  printf("y = ");
  printSet(y);
  nl();
  printf("x ∪ y = ");
  printSet(unionSet(x, y));
  nl();
  printf("x ∩ y = ");
  printSet(intersectSet(x, y));
  nl();
  printf("x − y = ");
  printSet(differenceSet(x, y));
  nl();
}
