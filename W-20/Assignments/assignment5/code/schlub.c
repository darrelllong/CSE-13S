#include "schlub.h"
#include "util.h"
#include <inttypes.h>
#include <stdbool.h>

#define GAPS 101

void schlub_sort(uint32_t *arr, uint32_t n) {
  const uint32_t gaps[] = {
    629856,   559872,   419904,   373248,   314928,   279936,   248832,
    236196,   209952,   186624,   157464,   139968,   124416,   118098,
    104976,    93312,    82944,    78732,    69984,    62208,    59049,
     52488,    46656,    41472,    39366,    34992,    31104,    27648,
     26244,    23328,    20736,    19683,    17496,    15552,    13824,
     13122,    11664,    10368,     9216,     8748,     7776,     6912,
      6561,     5832,     5184,     4608,     4374,     3888,     3456,
      3072,     2916,     2592,     2304,     2187,     1944,     1728,
      1536,     1458,     1296,     1152,     1024,      972,      864,
       768,      729,      648,      576,      512,      486,      432,
       384,      324,      288,      256,      243,      216,      192,
       162,      144,      128,      108,       96,       81,       72,
        64,       54,       48,       36,       32,       27,       24,
        18,       16,       12,        9,        8,        6,        4,
         3,        2,        1
  };

  for (uint8_t i = 0; i < GAPS; i += 1) {
    bool swapped;
    uint32_t passes = 1;
    uint32_t gap = gaps[i];

    do {
      swapped = false;

      for (uint32_t j = passes * gap; j < n; j += 1) {
        if (compare(arr[j], arr[j - gap])) {
          swap(&arr[j], &arr[j - gap]);
          swapped = true;
        }
      }

      passes += 1;
    } while (swapped);
  }

  return;
}
