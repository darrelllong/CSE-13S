#include "simon.h"

#define RL(x, y)  (((x) << (y)) | (x >> (64 - (y))))
#define RR(x, y)  (((x) >> (y)) | ((x) << (64 - (y))))

static uint64_t rk[72] = { 0 };

void simon_key_schedule(uint64_t K[]) {
  const uint8_t z[] = {
    1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0,
    1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1
  };

  rk[0] = K[0]; rk[1] = K[1]; rk[2] = K[2]; rk[3] = K[3];

  for (uint8_t i = 4; i < 72; i += 1) {
    uint64_t tmp = RR(rk[i - 1], 3);
    tmp = tmp ^ rk[i - 3];
    tmp = tmp ^ RR(tmp, 1);
    rk[i] = ~rk[i - 4] ^ tmp ^ z[(i - 4) % 62] ^ 3;
  }

  return;
}

void simon_encrypt(uint64_t Pt[], uint64_t Ct[]) {
  Ct[0] = Pt[0]; Ct[1] = Pt[1];

  for (uint8_t i = 0; i < 72; i += 1) {
    uint64_t tmp = Ct[1];
    Ct[1] = Ct[0] ^ (RL(Ct[1], 1) & RL(Ct[1], 8)) ^ RL(Ct[1], 2) ^ rk[i];
    Ct[0] = tmp;
  }

  return;
}

void simon_decrypt(uint64_t Ct[], uint64_t Pt[]) {
  Pt[0] = Ct[0]; Pt[1] = Ct[1];

  for (int8_t i = 71; i >= 0; i -= 1) {
    uint64_t tmp = Pt[0];
    Pt[0] = Pt[1] ^ (RL(Pt[0], 1) & RL(Pt[0], 8)) ^ RL(Pt[0], 2) ^ rk[i];
    Pt[1] = tmp;
  }

  return;
}

