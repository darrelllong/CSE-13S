#include <stdio.h>
#include <stdlib.h>

#define LENGTH  10

void unitV(double *v, int l) {
    double sum = 0.0;
    for (int i = 0; i < l; i += 1) {
        sum += v[i] < 0.0 ? -v[i] : v[i];
    }
    for (int i = 0; i < l; i += 1) {
        v[i] /= sum;
    }
    return;
}

int main(void) {
    double v[LENGTH];
    for (int i = 0; i < LENGTH; i += 1) {
        v[i] = (double) (random() & 63);
        printf("v[%d] = %lf\n", i, v[i]);
    }
    unitV(v, LENGTH);
    for (int i = 0; i < LENGTH; i += 1) {
        printf("v[%d] = %lf\n", i, v[i]);
    }
    return EXIT_SUCCESS;
}
