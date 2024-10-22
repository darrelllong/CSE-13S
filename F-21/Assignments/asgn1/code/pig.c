#include "names.h"
#include <limits.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SCORE 100
#define MIN_PLAYERS 2
#define MAX_PLAYERS 10
#define RAZORBACK_POINTS 10
#define TROTTER_POINTS 10
#define SNOUTER_POINTS 15
#define JOWLER_POINTS 5

// All possible die faces in a game of Pig.
typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Face;

// A single die in a game of Pig.
// 1/7 for everything except Side and Jowler, which are both 2/7.
const Face die[7] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

// Rolls the die, returning a random value within [0, 7).
Face roll(void) {
    return die[random() % 7];
}

// Returns the index of the next player in the circle.
unsigned next(unsigned pos, unsigned players) {
    return (pos + 1) % players;
}

int main(void) {
    // Get number of players.
    int64_t players = 0;
    printf("How many players? ");
    if (scanf("%" SCNd64, &players) != 1 || players < MIN_PLAYERS || players > MAX_PLAYERS) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        players = MIN_PLAYERS;
    }

    // Get random seed and set it.
    int64_t seed = 0;
    printf("Random seed: ");
    if (scanf("%" SCNd64, &seed) != 1 || seed < 0 || seed > UINT_MAX) {
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        seed = 2021;
    }
    srandom(seed);

    // Everyone starts off at 0 points.
    unsigned points[MAX_PLAYERS] = { 0 };

    for (unsigned i = 0;; i = next(i, players)) {
        Face rolled = SIDE;
        printf("%s rolls the pig...", names[i]);
        do {
            switch (rolled = roll()) {
            case SIDE:
                printf(" pig lands on side");
                break;
            case RAZORBACK:
                printf(" pig lands on back");
                points[i] += RAZORBACK_POINTS;
                break;
            case TROTTER:
                printf(" pig lands upright");
                points[i] += TROTTER_POINTS;
                break;
            case SNOUTER:
                printf(" pig lands on snout");
                points[i] += SNOUTER_POINTS;
                break;
            case JOWLER:
                printf(" pig lands on ear");
                points[i] += JOWLER_POINTS;
                break;
            }
        } while (rolled != SIDE && points[i] < MAX_SCORE);
        printf("\n");

        if (points[i] >= MAX_SCORE) {
            printf("%s wins with %d points!\n", names[i], points[i]);
            break;
        }
    }

    return EXIT_SUCCESS;
}
