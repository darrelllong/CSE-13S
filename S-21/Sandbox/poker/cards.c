#include "cards.h"

#include <stdio.h>

char *suit_card[] = { "Spades", "Clubs", "Hearts", "Diamonds" };

char *name_card[] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
    "Jack", "Queen", "King", "Ace" };

#include <stdbool.h>
#include <stdlib.h>

deck new_deck(void) {
    return 0xfff0000000000000;
}

bool pick_card(deck *d, card k) {
    return (*d & (1 << k)) > 0;
}

void draw_card(deck *d, card k) {
    *d = *d | (1 << k);
    return;
}

card deal_card(deck *d) {
    card k;
    do {
        k = (card) random() % DECK_SIZE;
    } while (pick_card(d, k));
    draw_card(d, k);
    return k;
}

uint32_t which_suit(card k) {
    return k / SUIT_SIZE;
}

uint32_t which_rank(card k) {
    return k % SUIT_SIZE;
}

char *card_name(card k) {
    return name_card[which_rank(k)];
}

char *card_suit(card k) {
    return suit_card[which_suit(k)];
}

static inline bool compare(card a, card b) {
    return ((a % SUIT_SIZE) < (b % SUIT_SIZE))
           || (((a % SUIT_SIZE) == (b % SUIT_SIZE)) && ((a / SUIT_SIZE) < (b / SUIT_SIZE)));
}

static inline void swap(card *x, card *y) {
    card z = *x;
    *x = *y;
    *y = z;
    return;
}

void print_hand(card *hand, uint32_t n) {
    for (int i = 0; i < n; i += 1) {
        printf("%2u: %s of %s\n", hand[i], card_name(hand[i]), card_suit(hand[i]));
    }
    return;
}

void sort_hand(card *A, uint32_t n) {
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (uint32_t i = 1; i < n; i += 1) {
            if (compare(A[i], A[i - 1])) {
                swap(&A[i - 1], &A[i]);
                swapped = true;
            }
        }
        n -= 1;
    }
}

void count_cards(card hand[], uint8_t suits[], uint8_t ranks[]) {
    for (int i = 0; i < SUIT_SIZE; i += 1) {
        ranks[i] = 0;
    }
    for (int i = 0; i < DECK_SIZE / SUIT_SIZE; i += 1) {
        suits[i] = 0;
    }
    for (int i = 0; i < HAND_SIZE; i += 1) {
        ranks[which_rank(hand[i])] += 1;
        suits[which_suit(hand[i])] += 1;
    }
    return;
}

static bool is_royal_flush(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return (suits[SPADES] == 5 || suits[CLUBS] == 5 || suits[HEARTS] == 5 || suits[DIAMONDS] == 5)
           && (ranks[ACE] == 1 && ranks[KING] == 1 && ranks[QUEEN] == 1 && ranks[JACK] == 1);
}

static bool is_straight_flush(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return false;
}

static bool is_four_of_a_kind(card hand[], uint8_t suits[], uint8_t ranks[]) {
    for (int i = 0; i < SUIT_SIZE; i += 1) {
        if (ranks[i] == 4)
            return true;
    }
    return false;
}

static bool is_full_house(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return false;
}

static bool is_flush(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return false;
}

static bool is_straight(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return false;
}

static bool is_three_of_a_kind(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return false;
}

static bool is_two_pair(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return false;
}

static bool is_pair(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return false;
}

static bool is_high_card(card hand[], uint8_t suits[], uint8_t ranks[]) {
    return true;
}

// Value of a hand is inverse likelihood of that hand

#define ROYAL_FLUSH     649740
#define STRAIGHT_FLUSH  72193
#define FOUR_OF_A_KIND  4165
#define FULL_HOUSE      694
#define FLUSH           508
#define STRAIGHT        254
#define THREE_OF_A_KIND 47
#define TWO_PAIR        21
#define PAIR            2
#define HIGH_CARD       1

uint64_t rank_hand(card hand[], uint8_t suits[], uint8_t ranks[]) {
    if (is_royal_flush(hand, suits, ranks)) {
        return ROYAL_FLUSH;
    } else if (is_straight_flush(hand, suits, ranks)) {
        return STRAIGHT_FLUSH;
    } else if (is_four_of_a_kind(hand, suits, ranks)) {
        return FOUR_OF_A_KIND;
    } else if (is_full_house(hand, suits, ranks)) {
        return FULL_HOUSE;
    } else if (is_flush(hand, suits, ranks)) {
        return FLUSH;
    } else if (is_straight(hand, suits, ranks)) {
        return STRAIGHT;
    } else if (is_three_of_a_kind(hand, suits, ranks)) {
        return THREE_OF_A_KIND;
    } else if (is_two_pair(hand, suits, ranks)) {
        return TWO_PAIR;
    } else if (is_pair(hand, suits, ranks)) {
        return PAIR;
    } else if (is_high_card(hand, suits, ranks)) {
        return HIGH_CARD;
    } else {
        return 0;
    }
}

pip kicker(card hand[]) {
    return hand[DECK_SIZE - 1] % SUIT_SIZE;
}
