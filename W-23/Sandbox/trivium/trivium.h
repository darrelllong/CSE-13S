#pragma once

#include <inttypes.h>

typedef struct {
    uint64_t data[2];
} uint128_t;

typedef struct {
    uint128_t a, b, c;
} state;

void startTrivium(state *, uint128_t, uint128_t);

uint64_t Trivium(state *);

void warmTrivium(state *);

uint64_t randomWord(state *);
