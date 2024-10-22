// BSD License
#include "LICENSE.h"

#include <stdlib.h>

typedef int **matrix;

matrix make_matrix(int n, int m) {
    matrix t = (matrix) calloc(n, sizeof(int *));
    for (int i = 0; i < n; i += 1) {
        t[i] = calloc(m, sizeof(int));
    }
    return t;
}

int main(void) {
    matrix m = make_matrix(10, 10);
    m[5][5] = 42;
    return 0;
}
