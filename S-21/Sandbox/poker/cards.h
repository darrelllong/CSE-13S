#pragma once

#include <stdint.h>

#define HAND_SIZE 5
#define SUIT_SIZE 13
#define DECK_SIZE 52

typedef enum zoot { SPADES = 0, CLUBS = 1, HEARTS = 2, DIAMONDS = 3 } zoot;

typedef enum pip {
    TWO = 0,
    THREE = 1,
    FOUR = 2,
    FIVE = 3,
    SIX = 4,
    SEVEN = 5,
    EIGHT = 6,
    NINE = 7,
    TEN = 8,
    JACK = 9,
    QUEEN = 10,
    KING = 11,
    ACE = 12
} pip;

typedef uint64_t deck;

typedef uint8_t card;

deck new_deck(void);

card deal_card(deck *);

char *card_name(card);

char *card_suit(card);

void sort_hand(card *, uint32_t);

void print_hand(card *, uint32_t);

uint32_t which_suit(card);

uint32_t which_rank(card);

void count_cards(card[], uint8_t[], uint8_t[]);

uint64_t rank_hand(card[], uint8_t[], uint8_t[]);

pip kicker(card[]);
