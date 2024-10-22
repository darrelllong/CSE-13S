#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;
faces die[] = {LEFT, RIGHT, CENTER, PASS, PASS, PASS};

int main(void) {
    
    int seed;
    printf("Random seed: ");
    scanf("%d", &seed);
    srand(seed);
    for (int i = 0; i < 20; i += 1) {
        int roll = rand() % 6;
        printf("random = %d, face = %d\n", roll, die[roll]);
    }
    return 0;
}
