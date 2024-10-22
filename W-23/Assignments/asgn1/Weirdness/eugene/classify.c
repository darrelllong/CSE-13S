#include "classify.h"

void factor(uint32_t n, Vec *factors) {
    vec_push(factors, 1);
    for (uint32_t i = 2; i < n; i += 1) {
        if (n % i == 0) {
            vec_push(factors, i);
        }
    }
}

uint32_t sum(Vec *factors) {
    uint32_t s = 0;
    for (size_t i = 0; i < vec_len(factors); i += 1) {
        s += vec_buf(factors)[i];
    }
    return s;
}

bool is_prime(Vec *factors) {
    return vec_len(factors) == 1;
}

bool is_composite(Vec *factors) {
    return !is_prime(factors);
}

bool is_abundant(uint32_t n, Vec *factors) {
    return sum(factors) > n;
}

bool is_deficient(uint32_t n, Vec *factors) {
    return sum(factors) < n;
}

bool is_perfect(uint32_t n, Vec *factors) {
    return sum(factors) == n;
}

bool is_semiperfect(uint32_t n, Vec *factors) {
    for (uint64_t i = 0; i < ((uint64_t)1 << vec_len(factors)); i += 1) {
        uint64_t sum = 0;
        for (size_t j = 0; j < vec_len(factors); j += 1) {
            if (i & (1 << j)) {
                sum += vec_buf(factors)[j];
            }
        }
        if (sum == n) {
            return true;
        }
    }
    return false;
}

bool is_weird(uint32_t n, Vec *factors) {
    return is_abundant(n, factors) && !is_semiperfect(n, factors);
}
