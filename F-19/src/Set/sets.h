#ifndef _SETS_DL
#define _SETS_DL

#include <stdint.h>

#define memberSet(x, s)                                                        \
  (typeof(s))({                                                                \
    const uint8_t bits = sizeof(s) * 8;                                        \
    const uint8_t mask = bits - 1;                                             \
    ((s & (0x1 << (x & mask))) >> (x & mask));                                 \
  })

#define insertSet(x, s)                                                        \
  (typeof(s))({                                                                \
    const uint8_t bits = sizeof(s) * 8;                                        \
    const uint8_t mask = bits - 1;                                             \
    (s | (0x1 << (x & mask)));                                                 \
  })

#define deleteSet(x, s)                                                        \
  (typeof(s))({                                                                \
    const uint8_t bits = sizeof(s) * 8;                                        \
    const uint8_t mask = bits - 1;                                             \
    (s & ~(0x1 << (x & mask)));                                                \
  })

#define unionSet(s, t) (s | t)

#define intersectSet(s, t) (s & t)

#define differenceSet(s, t) (s & ~t)

#endif
