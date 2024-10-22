#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const uint32_t max = 16;
static uint32_t n = 8;

static uint32_t a[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
static uint32_t b[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

static uint32_t chess_board[max][max];

static uint64_t count = 0;

bool attempt(uint32_t i, uint32_t x, uint32_t y) {
    uint32_t k = 0;
    bool quit;
    do {
        uint32_t u, v;
        count += 1;
#ifdef DEBUG
        if (count % 1000000000 == 0) printf("Try %" PRIu64 "\n", count);
#endif
        quit = false;
        u = x + a[k];
        v = y + b[k];
        if ((u < n && u >= 0) && (v < n && v >= 0)) { // On the board?
            if (chess_board[u][v] == 0) { // Vacant?
                chess_board[u][v] = i; // Place knight
                if (i < n * n) { // Done?
                    quit = attempt(i + 1, u, v);
                    if (quit == false) {
                        chess_board[u][v] = 0; // Back up and try again
                    }
                } else {
                    quit = true;
                }
            }
        }
        k += 1;
    } while (quit == false && k < 8);
    return quit;
}

static inline bool valid(char *s) {
    while (*s) {
        if (!isdigit(*s)) {
            return false;
        }
        s++;
    }
    return true;
}

int main(int argc, char **argv) {
    int c;
    
    while ((c = getopt(argc, argv, "n:")) != -1) {
        switch (c) {
            case 'n': { // Change run length
                if (valid(optarg)) {
                    n = strtoll(optarg, (void *)0, 10);
                    n = n <= max ? n : max;
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    
    chess_board[0][0] = 1;
    
    if (attempt(2, 0, 0)) {
        for (uint32_t i = 0; i < n; i += 1) {
            for (uint32_t j = 0; j < n; j += 1) {
                printf("\t%" PRIu32, chess_board[i][j]);
            }
            printf("\n");
        }
        printf("Solution required %" PRIu64 " tries.\n", count);
        return 0;
    } else {
        printf("No solution in %" PRIu64 " tries.\n", count);
        return -1;
    }
}
