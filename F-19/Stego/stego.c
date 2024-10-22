#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <sys/time.h>
#include <unistd.h>

//
// Generate a uniformly distributed variate.  We're assuming that we have at
// 64 bits to work with.
//

long double uniform() {

  uint64_t buffer;
  getentropy(&buffer, 8);
  return (long double)buffer / (long double)INT64_MAX;
}

//
// Erlang-k variate using the convolution method.
//

long double erlang(long double a, uint32_t k) {

  long double c;

  c = 1.0;
  for (uint32_t i = 0; i < k; i += 1) {
    c *= uniform();
  }

  return -a * logl(c);
}

//
// Exponential variate using the inverse transform method.
//

long double exponential(long double a) { return -a * logl(uniform()); }

//
// Weibull variate using the inverse transform method.
//

long double weibull(long double a, long double b) {
  return a * powl(logl(uniform()), 1 / b);
}

#define STEADYSTATE 100
#define RUNLENGTH 100
#define PERIOD 600.0
#define SPACING 10.0
#define MEAN 2.0

static inline bool valid(char *s) {
  while (*s) {
    if (!isnumber(*s)) {
      return false;
    }
    s++;
  }
  return true;
}

static int code[8][3] = {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
                         {1, -1, -1},  {1, -1, 1},  {1, 1, -1},  {1, 1, 1}};

int main(int argc, char **argv) {
  uint32_t warmUp = STEADYSTATE;
  uint32_t run = RUNLENGTH;
  uint32_t msg = 1;
  int c;
  long double period = PERIOD;
  long double spacing = SPACING;
  long double noise = 1.0;
  long double time = 0.0;
  while ((c = getopt(argc, argv, "-m:r:w:p:s:n:")) != -1) {
    switch (c) {
    case 'm': { // Message [0, 7]
      if (valid(optarg)) {
        msg = 0x7 & strtoll(optarg, (void *)0, 10);
      }
      break;
    }
    case 'r': { // Run length
      if (valid(optarg)) {
        run = strtoll(optarg, (void *)0, 10);
      }
      break;
    }
    case 'w': { // Steady State warm up
      if (valid(optarg)) {
        warmUp = strtoll(optarg, (void *)0, 10);
      }
      break;
    }
    case 'p': { // Period between groups
      if (valid(optarg)) {
        period = strtold(optarg, (void *)0);
      }
      break;
    }
    case 's': { // Spacing in the group
      if (valid(optarg)) {
        spacing = strtold(optarg, (void *)0);
      }
      break;
    }
    case 'n': { // Noise mean
      if (valid(optarg)) {
        noise = strtold(optarg, (void *)0);
      }
      break;
    }
    }
  }

  for (uint32_t i = 0; i < warmUp; i += 1) {
    printf("%10.18Lf\n", time);                  // First packet
    time += spacing + noise * exponential(MEAN); // Packet gap
    printf("%10.18Lf\n", time);                  // Second Packet
    time += spacing + noise * exponential(MEAN); // Packet gap
    printf("%10.18Lf\n", time);                  // Third packet
    time += spacing + noise * exponential(MEAN); // Packet gap
    printf("%10.18Lf\n", time);                  // Fourth packet
    time += period + noise * exponential(MEAN);  // Group gap

  }

  for (uint32_t i = 0; i < run; i += 1) {
    printf("%10.18Lf\n", time);                                 // First packet
    time += spacing * code[msg][0] + noise * exponential(MEAN); // First bit
    printf("%10.18Lf\n", time);                                 // Second Packet
    time += spacing * code[msg][1] + noise * exponential(MEAN); // Second bit
    printf("%10.18Lf\n", time);                                 // Third packet
    time += spacing * code[msg][2] + noise * exponential(MEAN); // Third bit
    printf("%10.18Lf\n", time);                                 // Fourth packet
    time += period + noise * exponential(MEAN);                 // Group gap
  }

  return 0;
}
