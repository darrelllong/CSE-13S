// BSD License
#include "LICENSE.h"

#include "bv64.h"
#include <stdint.h>

void sieve(bitV *v) {
    oddVec(v); // Only odd numbers, aside from 2, can be prime
    clrBit(v, 0); // 0 is, well, zero
    clrBit(v, 1); // 1 is unity
    setBit(v, 2); // 2 is prime
    for (uint32_t i = 2; i * i <= lenVec(v); i += 1) {
        if (getBit(v, i)) // It's prime
        {
            for (uint32_t k = 0; (k + i) * i <= lenVec(v); k += 1) {
                clrBit(v, (k + i) * i); // Its multiple are composite
            }
        }
    }
    return;
}
