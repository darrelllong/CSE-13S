#include "philos.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum faciem { PASS, LEFT, RIGHT, CENTER } faces;
faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };

static inline faces roll(void) {
    return die[random() % 6];
}

static inline uint8_t left(uint8_t pos, uint8_t players) {
    return (pos + players - 1) % players;
}

static inline uint8_t right(uint8_t pos, uint8_t players) {
    return (pos + 1) % players;
}

#define PHILOSOPHERS 14

int main(void) {
    int64_t seed = 0;
    printf("Random seed: ");
    if (scanf("%" SCNd64, &seed) != 1 || seed < 0) {
        fprintf(stderr, "Pseudorandom seed must be non-negative (%" PRId64 ").\n", seed);
        return 1;
    }
    srandom(seed);

    uint8_t players = 0;
    printf("How many players? ");
    if (scanf("%" SCNu8, &players) != 1 || players < 1 || players > PHILOSOPHERS) {
        fprintf(stderr, "Number of players must be from 1 to %d.\n", PHILOSOPHERS);
        return 1;
    }

    uint8_t pot = 0;
    uint8_t pos = 0;
    uint8_t bank[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

    for (;;) {
        if (bank[pos]) {
            if (bank[pos] + pot == 3 * players) {
                printf("%s wins the $%" PRIu8 " pot with $%" PRIu8 " left in the "
                        "bank!\n", philosophers[pos], pot, bank[pos]);
                break;
            }
            printf("%s rolls...", philosophers[pos]);
            uint8_t rolls = (bank[pos] >= 3) ? 3 : bank[pos];
            for (uint8_t i = 0; i < rolls; i += 1) {
                switch (roll()) {
                case LEFT:
                    bank[pos] -= 1;
                    bank[left(pos, players)] += 1;
                    printf(" gives $1 to %s", philosophers[left(pos, players)]);
                    break;
                case RIGHT:
                    bank[pos] -= 1;
                    bank[right(pos, players)] += 1;
                    printf(" gives $1 to %s", philosophers[right(pos, players)]);
                    break;
                case CENTER:
                    bank[pos] -= 1;
                    pot += 1;
                    printf(" puts $1 in the pot");
                    break;
                case PASS:
                    printf(" gets a pass");
                    break;
                }
            }
            printf("\n");
        }
        pos = right(pos, players);
    }

    return 0;
}
