#define TSTS 10000000

#include <stddef.h>
#include <stdint.h>

// Ray Beaulieu, Stefan Treatman-Clark, Douglas Shors, Bryan Weeks, Jason
// Smith and Louis Wingers. "The SIMON and SPECK lightweight block ciphers,"
// In proceedings of the Design Automation Conference (DAC),
// 2015 52nd ACM/EDAC/IEEE, pp. 1-6. IEEE, 2015.

#define LCS(X, K)                                                              \
  (X << K) | (X >> (sizeof(uint64_t) * 8 - K)) // left circular shift
#define RCS(X, K)                                                              \
  (X >> K) | (X << (sizeof(uint64_t) * 8 - K)) // right circular shift

// Core SPECK operation
#define R(x, y, k) (x = RCS(x, 8), x += y, x ^= k, y = LCS(y, 3), y ^= x)

void Speck128ExpandKeyAndEncrypt(uint64_t pt[], uint64_t ct[], uint64_t K[]) {
  uint64_t A = K[0],
           B = K[1];
  ct[0] = pt[0];
  ct[1] = pt[1];
  for (size_t i = 0; i < 32; i += 1) {
    R(ct[1], ct[0], A);
    R(B, A, i);
  }
  return;
}

uint64_t random64(uint64_t entropy[], uint64_t key[]) {

  Speck128ExpandKeyAndEncrypt(entropy, entropy, key); // Encrypt 16 bytes

  return entropy[0] ^ entropy[1]; // XOR them together for a 64 bit result
}

#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdio.h>

static inline bool valid(char *s) {
  while (*s) {
    if (!isdigit(*s)) {
      return false;
    }
    s++;
  }
  return true;
}

int main(int argc, char **argv) {
  uint64_t entropy[2] = {0xb65d5bff1c9cd747, 0x0536d9ce768574c2}; // Dr Thompson
  uint64_t key    [2] = {0xb1a84340c5fda1e5, 0x4b7049f8cb1b738a}; // Dr Johnson
  uint64_t count [64] = {0};
  uint64_t tests = TSTS;
  int c;
  bool quiet = false;

  while ((c = getopt(argc, argv, "-qtn:")) != -1) {
    switch (c) {
      case 'n': { // Change run length
        if (valid(optarg)) {
          tests = strtoll(optarg, (void *) 0, 10);
        } else {
          tests = TSTS;
        }
        break;
      }
      case 't': { // Seed with current time
        struct timeval t;
        gettimeofday(&t, (void *) 0);
        entropy[0] = (uint64_t) t.tv_sec;
        entropy[1] = (uint64_t) t.tv_usec;
        break;
      }
      case 'q' : { // Shh! Be verwy verwy quiet!
        quiet = true;
      }
    }
  }

  for (uint64_t i = 0; i < tests; i += 1) {
    uint64_t r = random64(entropy, key);
    // Tally bits
    for (int j = 0; j < 64; j += 1) {
      count[j] += r & 0x1;
      r >>= 1;
    }
  }

  for (int i = 0; i < 64; i += 1) {
    if (quiet) {
      printf("%+2.12Lf\n", 0.5 - count[i] / (long double) tests);
    } else {
      printf("c[%d] = %" PRIu64 " ➔ %+2.12Lf" "\n",
             i, count[i], 0.5 - count[i] / (long double) tests);
    }
  }
  return 0;
}
