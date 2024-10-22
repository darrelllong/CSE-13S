#include "names.h"

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DIE_FACES        7
#define MAX_SCORE        100
#define MIN_PLAYERS      2
#define MAX_PLAYERS      10
#define RAZORBACK_POINTS 10
#define TROTTER_POINTS   10
#define SNOUTER_POINTS   15
#define JOWLER_POINTS    5

typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Face;

const Face die[] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

Face roll(void) {
    return die[random() % DIE_FACES];
}

uint32_t next(uint32_t pos, uint32_t players) {
    return (pos + 1) % players;
}

int main(void) {

    const uint32_t scores[]
        = { 0, 0, RAZORBACK_POINTS, TROTTER_POINTS, SNOUTER_POINTS, JOWLER_POINTS, JOWLER_POINTS };

    const char *actions[] = { "on side", "on back", "upright", "on snout", "on ear" };

    uint32_t points[MAX_PLAYERS] = { 0 };

    int32_t player_count;
    printf("How many players? ");
    if (scanf("%" SCNd32, &player_count) != 1 || player_count < MIN_PLAYERS
        || player_count > MAX_PLAYERS) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        player_count = MIN_PLAYERS;
    }

    int64_t reed = 0;
    uint32_t seed = 0; // Correct type rather than lower bytes
    printf("Random seed: ");
    if (scanf("%" SCNd64, &reed) != 1 || reed < 0 || reed > UINT_MAX) {
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        seed = 2021;
    } else {
        seed = reed;
    }
    srandom(seed);

    bool no_winner = true;
    uint32_t winner = 0, player = 0;
    do {
        Face die;
        printf("%s rolls the pig...", names[player]);
        do {
            die = roll();
            points[player] += scores[die];
            printf(" pig lands %s", actions[die]);
        } while (die != SIDE && points[player] < MAX_SCORE);
        printf("\n");
        if (points[player] >= MAX_SCORE) {
            no_winner = false;
            winner = player;
        }
        player = next(player, player_count);
    } while (no_winner);
    printf("%s wins with %" PRIu32 " points!\n", names[winner], points[winner]);
    return 0;
}
