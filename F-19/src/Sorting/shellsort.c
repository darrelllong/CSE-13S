#include "shellsort.h"
#include <stdint.h>

//
// shellSort
//
// Sort by using decreasing increments. The expected running time is improved
// since in the beginning out of order elements are moved long distances.
//
// This sorting routine is surprising poorly understood.
//
// See: http://www.cs.princeton.edu/~rs/shell/paperF.pdf
//

void shellSort(uint32_t data[], int length) {

  // Platt sequence -- best one that I know.

  const int hl  = 100;
  const int h[] = { 1, 2, 3, 4, 6, 8, 9, 12, 16, 18, 24, 27, 32, 36, 48, 54, 64, 72,
    81, 96, 108, 128, 144, 162, 192, 216, 243, 256, 288, 324, 384, 432, 486,
    512, 576, 648, 729, 768, 864, 972, 1024, 1152, 1296, 1458, 1536, 1728, 1944,
    2187, 2304, 2592, 2916, 3072, 3456, 3888, 4374, 4608, 5184, 5832, 6561,
    6912, 7776, 8748, 9216, 10368, 11664, 13122, 13824, 15552, 17496, 19683,
    20736, 23328, 26244, 27648, 31104, 34992, 39366, 41472, 46656, 52488, 59049,
    62208, 69984, 78732, 82944, 93312, 104976, 118098, 124416, 139968, 157464,
    186624, 209952, 236196, 248832, 279936, 314928, 373248, 419904, 472392,
    559872, 629856 };

  for (int s = hl - 1; s >= 0; s -= 1) {
    int step = h[s];

    for (int j = step; j < length; j += 1) {
      uint32_t key = data[j];
      int i = j - step;

      moves += 1;

      while (++compares && i >= 0 && data[i] > key) {
        data[i + step] = data[i];
        i -= step;
        moves += 1;
      }
      data[i + step] = key;

      moves += 1;
    }
  }
  return;
}
