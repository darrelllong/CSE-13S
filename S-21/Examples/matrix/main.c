#include "matrix.h"

int main() {
	Matrix *a = mat_create(5,5);
	Matrix *b = mat_create(5,5);

	int x = 1;
	for (int i = 0; i < mat_rows(a); i++){
		for (int j = 0; j < mat_cols(a); j++){
			mat_set_cell(a, i, j, x++);
		}
	}

	for (int i = 0; i < mat_rows(b); i++){
		for (int j = 0; j < mat_cols(b); j++){
			mat_set_cell(b, i, j, x++);
		}
	}

	Matrix *c = mat_multiply(a, b);
	
	mat_print(a);
	mat_print(b);
	mat_print(c);
	
	return 0;
}
