#ifndef __MATRIX_H
#define __MATRIX_H

typedef struct Matrix Matrix;

Matrix *mat_create(int r, int c);

int mat_rows(Matrix *m);

int mat_cols(Matrix *m);

int mat_get_cell(Matrix *m, int r, int c);

void mat_set_cell(Matrix *m, int r, int c, int x);

Matrix *mat_multiply(Matrix *a, Matrix *b);

void mat_print(Matrix *m);

#endif
