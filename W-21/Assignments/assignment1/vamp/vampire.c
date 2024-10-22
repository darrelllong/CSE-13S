// Darrell Long
// © 2021

#include "names.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// Each vampire will roll two 6-sided dice if they are alive (perhaps we should say
// undead), going around the circle in sequence. Only undead vampires roll.
// Dead vampires do not roll dice. It is possible, however, for vampires to be
// resurrected, so death does not mean permanent ejection from the game.
// The vampire who has the lowest roll during the round loses a life as a result
// of being forced to ingest garlic.
//
// Be careful—if there are two or more vampires who get the same lowest roll, it is
// the first unlucky vampire who must eat the garlic.
//
// Rolling two sixes results in special activity: the vampires on the immediate left
// and right of the vampire who rolled the sixes resurrect or sparkle.
//
// A vampire is resurrected if they were previously dead, meaning they had zero lives.
// Since each vampire rolls the dice following the sequence of the circle, it is possible
// for a vampire to be resurrected after their turn to roll the dice. They do not roll
// the dice in this event.
//
// A vampire sparkles if they are still alive.
//
// In either case, the neighboring vampires will gain 1 life. Nothing of note occurs
// for the vampire who rolled the sixes.

#define MIDNIGHT 10 // Dice are 0 to 5, not 1 to 6. So, the maximum roll of 2 dice is 10.
#define LIVES    3 // Vampires start off with 3 lives, like all good game characters.

// The behavior of the % operator in C is undefined for negative integers, so we need to force
// it to compute modulus rather than remainder.

static inline uint32_t succ(uint32_t k, uint32_t n) {
    assert(k >= 0 && k < n);
    return (k + n + 1) % n;
}

static inline uint32_t pred(uint32_t k, uint32_t n) {
    assert(k >= 0 && k < n);
    return (k + n - 1) % n;
}

static inline uint32_t roll(uint32_t n) {
    return random() % n;
}

int main(void) {
    uint32_t lives[] = { LIVES, LIVES, LIVES, LIVES, LIVES, LIVES, LIVES, LIVES, LIVES, LIVES };
    uint32_t seed = 2021, vamps = VAMPS; // In case we want defaults

    printf("Number of players: ");
    if ((scanf("%u", &vamps) < 1) || vamps < 2 || vamps > VAMPS) {
        fprintf(stderr, "Invalid number of players.\n");
        return 1;
    }

    printf("Random seed: ");
    if (scanf("%u", &seed) < 1) { // unsigned means it cannot be negative if valid
        fprintf(stderr, "Invalid random seed.\n");
        return 1;
    }
    srandom(seed); // Set starting point for pseudorandom number generator

    for (uint32_t alive = vamps, trial = 1; alive > 1; trial += 1) {
        uint32_t smallest = MIDNIGHT + 1, eater;

        printf("Round %u\n", trial);

        for (uint32_t i = 0; i < vamps; i += 1) {
            if (lives[i]) { // Only the animate may roll

                uint32_t die_one = roll(6), die_two = roll(6); // Roll them bones

                printf(" - %s rolls %s... ", names[i], rolls[die_one][die_two]);

                if (die_one + die_two == MIDNIGHT) { // You lucky little fang-face!

                    uint32_t left = pred(i, vamps), right = succ(i, vamps); // Clown: L, Joker: R

                    if (lives[left]++ > 0) { // Dr. Hahnemann sets to work
                        printf("%s sparkles ", names[left]);
                    } else {
                        printf("%s resurrects ", names[left]);
                        alive += 1;
                    }

                    if (lives[right]++ > 0) { // Dr. Hahnemann sets to work
                        printf("%s sparkles ", names[right]);
                    } else {
                        printf("%s resurrects ", names[right]);
                        alive += 1;
                    }
                } else if (die_one + die_two < smallest) { // Enjoy the garlic!
                    smallest = die_one + die_two;
                    eater = i;
                }
                printf("\n");
            }
        }

        printf("%s is forced to eat garlic!\n", names[eater]); // Consequences
        lives[eater] -= 1;
        if (lives[eater] == 0) {
            printf("%s has died.\n", names[eater]);
            alive -= 1;
        } else {
            printf("%s has %u %s remaining.\n", names[eater], lives[eater],
                lives[eater] == 1 ? "life" : "lives");
        }
    }

    for (uint32_t i = 0; i < vamps; i += 1) { // Like Highlanders, there can only be one.
        if (lives[i] > 0) {
            printf("%s won the garlic game!\n", names[i]);
        }
    }
    return 0;
}
