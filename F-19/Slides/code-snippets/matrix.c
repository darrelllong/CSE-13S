#include <stdio.h>
#include <stdlib.h>

int **matrix_create(int n) {
  int **m = (int **)calloc(n, sizeof(int *));
  if (m) {
    for (int i = 0; i < n; ++i) {
      m[i] = (int *)calloc(n, sizeof(int));
    }

    return m;
  }

  return NULL;
}

void matrix_delete(int **m, int n) {
  for (int i = 0; i < n; ++i) {
    free(m[i]);
    m[i] = NULL;
  }

  free(m);
  m = NULL;
  return;
}

int main(void) {
  int n = 10;
  int **m = matrix_create(n);

  // Prints zerored out 10x10 matrix.
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      printf("%d ", m[i][j]);
    }

    printf("\n");
  }

  matrix_delete(m, n);
  return 0;
}

