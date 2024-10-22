#include "cards.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    card hand[5];
    deck d = new_deck();
    srandom((unsigned) time((time_t *) 0));
    for (int i = 0; i < 5; i += 1) {
        hand[i] = deal_card(&d);
    }

    print_hand(hand, 5);
    puts("--------------------");
    sort_hand(hand, 5);
    print_hand(hand, 5);

    uint8_t ranks[SUIT_SIZE], suits[DECK_SIZE / SUIT_SIZE];
    count_cards(hand, suits, ranks);

    printf("Suits: ");
    for (int i = 0; i < DECK_SIZE / SUIT_SIZE; i += 1) {
        printf("%2u ", suits[i]);
    }
    printf("\n");
    printf("Ranks: ");
    for (int i = 0; i < SUIT_SIZE; i += 1) {
        printf("%2u ", ranks[i]);
    }
    printf("\n");

    return 0;
}
