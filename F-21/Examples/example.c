#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

static inline bool even(int64_t n) {
    return n % 2 == 0;
}

static inline bool odd(int64_t n) {
    return n % 2 == 1;
}

static inline int64_t succ(int64_t k, int64_t n) {
    assert(n > 0);
    return (k + 1 + n) % n;
}

static inline int64_t pred(int64_t k, int64_t n) {
    assert(n > 0);
    return (k - 1 + n) % n;
}



