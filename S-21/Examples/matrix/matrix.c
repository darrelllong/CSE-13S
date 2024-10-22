#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct Matrix {
  int rows;
  int cols;
  int **data;
};

Matrix *mat_create(int r, int c) {
  Matrix *m = (Matrix *)calloc(1, sizeof(Matrix));
  m->rows = r;
  m->cols = c;
  m->data = (int **)calloc(m->rows, sizeof(int *));
  for (int i = 0; i < m->rows; i++){
    m->data[i] = (int *)calloc(m->cols, sizeof(int));
  }
  return m;
}

int mat_rows(Matrix *m){
  return m->rows;
}

int mat_cols(Matrix *m) {
  return m->cols;
}

int mat_get_cell(Matrix *m, int r, int c){
  return m->data[r][c];
}

void mat_set_cell(Matrix *m, int r, int c, int x){
  m->data[r][c] = x;
}

Matrix *mat_multiply(Matrix *a, Matrix *b) {
  assert(a->cols == b->rows);

  Matrix *c = mat_create(a->rows, b->cols);

  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < b->cols; j++) {
      int sum = 0;
      for (int k = 0; k < a->cols; k++) {
        sum += mat_get_cell(a, i, k) * mat_get_cell(b, k, j);
      }
      mat_set_cell(c, i, j, sum);
    }
  }
  return c;
}

void mat_print(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      printf("%d ", m->data[i][j]);
    }
    printf("\n");
  }
}




