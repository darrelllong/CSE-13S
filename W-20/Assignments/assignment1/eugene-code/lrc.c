#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;
faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };

faces roll_dice(void) {
  return die[rand() % 6];
}

uint8_t left(uint8_t pos, uint8_t players) {
  return (pos + players - 1) % players;
}

uint8_t right(uint8_t pos, uint8_t players) {
  return (pos + 1) % players;
}

int main(void) {
  uint32_t seed = 0;
  printf("Random seed: ");
  scanf("%" SCNu32, &seed);
  srand(seed);

  uint8_t players = 0;
  printf("How many players? ");
  scanf("%" SCNu8, &players);

  if (players < 2 || players > 10) {
    fprintf(stderr, "LRC can only be played with 2-10 players!\n");
    return EXIT_FAILURE;
  }

  uint8_t pot = 0;
  uint8_t pos = 0;
  uint8_t bank[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

  const char *names [] = {
    "Dude",   "Walter", "Maude",   "Big Lebowski", "Brandt",
    "Bunny",  "Karl",   "Kieffer", "Franz",        "Smokey"
  };

  for (;;) {
    if (bank[pos]) {
      if (bank[pos] + pot == 3 * players) {
        printf("%s wins the $%" PRIu8 " pot with $%" PRIu8 " left in the"
          "bank!\n", names[pos], pot, bank[pos]);
        break;
      }

      printf("%s rolls...", names[pos]);
      uint8_t rolls = (bank[pos] >= 3) ? 3 : bank[pos];

      for (uint8_t i = 0; i < rolls; i += 1) {
        switch (roll_dice()) {
        case LEFT:
          bank[pos] -= 1;
          bank[left(pos, players)] += 1;
          printf(" gives $1 to %s", names[left(pos, players)]);
          break;
        case RIGHT:
          bank[pos] -= 1;
          bank[right(pos, players)] += 1;
          printf(" gives $1 to %s", names[right(pos, players)]);
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

  return EXIT_SUCCESS;
}
