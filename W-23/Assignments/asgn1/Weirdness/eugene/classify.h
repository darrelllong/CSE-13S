#pragma once

#include "vec.h"
#include <stdbool.h>
#include <stdint.h>

void factor(uint32_t n, Vec *factors);

uint32_t sum(Vec *factors);

bool is_prime(Vec *factors);

bool is_composite(Vec *factors);

bool is_abundant(uint32_t n, Vec *factors);

bool is_deficient(uint32_t n, Vec *factors);

bool is_perfect(uint32_t n, Vec *factors);

bool is_semiperfect(uint32_t n, Vec *factors);

bool is_weird(uint32_t n, Vec *factors);
