#ifndef __SETS_H__
#define __SETS_H__

#include <stdint.h>

#define set_member(s, x)                    \
    (__typeof__(s))({                       \
        const uint8_t bits = sizeof(s) * 8; \
        const uint8_t mask = bits - 1;      \
        ((s >> (x & mask)) & 1);            \
    })

#define set_insert(s, x)                    \
    (__typeof__(s))({                       \
        const uint8_t bits = sizeof(s) * 8; \
        const uint8_t mask = bits - 1;      \
        (s | (1 << (x & mask)));            \
    })

#define set_remove(s, x)                    \
    (__typeof__(s))({                       \
        const uint8_t bits = sizeof(s) * 8; \
        const uint8_t mask = bits - 1;      \
        (s & ~(1 << (x & mask)));           \
    })

#define set_all(s)                          \
    (__typeof__(s))({                       \
        (sizeof(s) * 8 - 1);                \
    })

#endif
