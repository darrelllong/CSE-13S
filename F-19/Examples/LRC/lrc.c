#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint32_t min(uint32_t a, uint32_t b) { return (a < b) ? a : b; }

static inline uint32_t left(uint32_t k, uint32_t p) { return (k + p - 1) % p; }

static inline uint32_t right(uint32_t k, uint32_t p) { return (k + 1) % p; }

typedef enum faciem { LEFT = 0, RIGHT, CENTER, PASS } faces;

faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };

static inline faces roll(void) { return die[random() % 6]; }

const uint32_t max  = 10; // Maximum number of players
const uint32_t cash =  3; // $3 is the normal starting fund

const char *names[] = { "Whoopi", "Dale",  "Rosie", "Jimmie", "Barbara", 
                        "Kyle",   "Raven", "Tony",  "Jenny",  "Clint" };

int main(void) {

  uint32_t bank[] = { cash, cash, cash, cash, cash, 
                      cash, cash, cash, cash, cash }; // Each player's $

  uint32_t pot = 0; // No winnings yet!
  uint32_t seed;    // Where to start the PRNG
  uint32_t players; // The total number of players
  uint32_t active;  // the number that are currently active.

  printf("Random seed: "); fflush(stdout); scanf("%u", &seed);
  printf("How many players? "); fflush(stdout); scanf("%u", &players);

  if (players > max) {
    printf("Maximum players is %d.\n", max);
    exit(1);
  } else if (players < 1) {
    printf("There must be at least one player.\n");
    exit(1);
  }

  active = players; // We all play!

  srandom(seed);

  do {
    for (uint32_t i = 0; i < players && active > 1; i += 1) {

      uint32_t hand = min(bank[i], 3); // There are at most 3 dice

      if (hand > 0) {
        printf("%s rolls...", names[i]);
        while (hand > 0) {
          switch (roll()) {

          case LEFT: {
            printf(" gives $1 to %s", names[left(i, players)]);
            if (bank[left(i, players)] == 0) {
              active += 1; // Revive!
            }
            bank[left(i, players)] += 1;
            bank[i] -= 1; // $1 to the left
            break;
          }

          case RIGHT: {
            printf(" gives $1 to %s", names[right(i, players)]);
            if (bank[right(i, players)] == 0) {
              active += 1; // Revive!
            }
            bank[right(i, players)] += 1;
            bank[i] -= 1; // $1 to the right
            break;
          }

          case CENTER: {
            printf(" puts $1 in the pot");
            pot += 1;
            bank[i] -= 1;
            break; // $1 to the pot
          }

          case PASS: {
            printf(" gets a pass");
            break;
          }
          }

          hand -= 1; // Next die

          if (bank[i] == 0) {
            active -= 1; // Player has gone broke!
          }
        }
        printf("\n");
      }
    }
  } while (active > 1);

  for (uint32_t i = 0; i < players; i += 1) {
    if (bank[i] > 0) {
      printf("%s wins the $%d pot with $%d left in the bank!\n", names[i], pot,
          bank[i]);
    }
  }
  return 0;
}
