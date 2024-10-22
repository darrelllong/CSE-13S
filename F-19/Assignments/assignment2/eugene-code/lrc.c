#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_CASH 3
#define MIN_PLAYERS 1
#define MAX_PLAYERS 10

// Typedef enum for faces of a die in LRC.
typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;
faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };

// Player names.
const char *names[] = { "Whoopi", "Dale", "Rosie", "Jimmie", "Barbara", "Kyle",
  "Raven", "Tony", "Jenny", "Clint" };

faces roll_dice(void) {
  return die[rand() % 6];
}

void pass_left(uint8_t pos, uint8_t *money, uint8_t players, uint8_t *active) {
  uint8_t left = (pos + players - 1) % players;
  if (money[left] == 0) {
    *active += 1;
  }

  money[left] += 1;
  money[pos] -= 1;
  printf(" passes $1 to %s", names[left]);
  return;
}

void pass_right(uint8_t pos, uint8_t *money, uint8_t players, uint8_t *active) {
  uint8_t right = (pos + 1) % players;
  if (money[right] == 0) {
    *active += 1;
  }

  money[right] += 1;
  money[pos] -= 1;
  printf(" passes $1 to %s", names[right]);
  return;
}

void pass_center(uint8_t pos, uint8_t *money, uint8_t *pot) {
  money[pos] -= 1;
  *pot += 1;
  printf(" puts $1 in the pot");
  return;
}

int main(void) {
  // Get random seed.
  uint32_t rand_seed = 0;
  printf("Random seed: ");
  scanf("%" PRIu32, &rand_seed);
  srand(rand_seed);

  // Get and sanitize number of players.
  uint8_t players = 0;
  printf("Number of players: ");
  scanf("%" SCNu8, &players);
  assert(players >= MIN_PLAYERS && players <= MAX_PLAYERS);

  // LRC global player pot and player money;
  // uint8_t since there can only be $30 max in a game.
  uint8_t pot = 0;
  uint8_t money[players];

  // Each player starts with $3.
  // Can't use memset for uint8_t's.
  for (uint8_t i = 0; i < players; ++i) {
    money[i] = START_CASH;
  }

  // Counters to keep track of current and active players.
  uint8_t active = players;
  uint8_t player = 0;

  while (active != 1) {
    if (money[player] > 0) {
      printf("%s rolls...", names[player]);

      // Calculate how many rolls current player gets.
      uint8_t rolls = (money[player] >= 3) ? 3 : money[player];

      // Roll the die.
      for (uint8_t i = 0; i < rolls && active != 1; ++i) {
        switch (roll_dice()) {
        case LEFT:
          pass_left(player, money, players, &active);
          break;
        case RIGHT:
          pass_right(player, money, players, &active);
          break;
        case CENTER:
          pass_center(player, money, &pot);
          break;
        case PASS:
          printf(" gets a pass");
          break;
        }

        // Did player go broke?
        if (money[player] == 0) {
          active -= 1;
        }
      }

      // Print newline for formatting.
      printf("\n");
    }

    // Move onto next player.
    player = (player + 1) % players;
  }

  // Find and celebrate the winner.
  uint8_t winner = 0;
  while (money[winner] == 0) {
    winner += 1;
  }

  printf("%s wins the $%" PRIu8 " pot with $%" PRIu8 " left in the bank!\n",
      names[winner], pot, money[winner]);

  return 0;
}
