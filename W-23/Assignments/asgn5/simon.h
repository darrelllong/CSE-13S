#ifndef __SIMON_H__
#define __SIMON_H__

#include <inttypes.h>

void simon_key_schedule(uint64_t K[]);

void simon_encrypt(uint64_t Pt[], uint64_t Ct[]);

void simon_decrypt(uint64_t Ct[], uint64_t Pt[]);

#endif

