#include <inttypes.h>
#include <stdio.h>

// All known Barker sequences (modulo negations and reversals) which are isomorphic to
// these sequences.
//
// int8_t b[] = {+1, -1};
// int8_t b[] = {+1, +1};
// int8_t b[] = {+1, +1, -1};
// int8_t b[] = {+1, +1, -1, +1};
// int8_t b[] = {+1, +1, +1, -1};
// int8_t b[] = {+1, +1, +1, -1, +1};
// int8_t b[] = { +1, +1, +1, -1, -1, +1, -1 };
//int8_t b[] = {+1, +1, +1, -1, -1, -1, +1, -1, -1, +1, -1}; // 802.11 standard
int8_t b[] = {+1, +1, +1, +1, +1, -1, -1, +1, +1, -1, +1, -1, +1}; // Longest
//int8_t b[] = {+1, +1, -1, -1, -1, 0, +1, 0, 0, 0, 0, 0, -1, 0, +1, -1, -1}; // random

int8_t autocorrelate(int8_t v) {
    int8_t sum = 0, l = sizeof(b);
    for (int i = 0; i < l; i += 1) {
        sum += b[i] * b[(i + v) % l];
    }
    return sum;
}

int main(void) {
    for (int8_t i = 0; i < 2 * sizeof(b); i += 1) {
        printf("%" PRIi8 ": %" PRIi8 "\n", i, autocorrelate(i));
    }
    return 0;
}
