// BSD License
#include "LICENSE.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    uint8_t b[4096]; // 4K
    int l = 0;
    while ((l = read(0, b, 4096)) > 0) {
        for (int i = 0; i < l; i += 1) {
            printf("%2u\n", b[i]);
        }
    }
    return 0;
}
