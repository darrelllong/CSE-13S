// BSD License
#include "LICENSE.h"

#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>

#define BLK 1024

bool buffered_write(int file, char b[], int l, bool flush) {
    static char buffer[BLK];
    static int blkP = 0;
    for (uint32_t i = 0; i < l; i += 1) {
        buffer[blkP] = b[i];
        blkP += 1;
        if (blkP == BLK) { // Buffer is full
            if (write(file, buffer, BLK) < BLK) { // Clear the buffer
                return false; // write failed
            }
            blkP = 0;
        }
    }
    if (flush && blkP > 0) { // Flush the buffer
        if (write(file, buffer, blkP) < blkP) {
            return false; // write failed
        }
        blkP = 0;
    }
    return true;
}

int main(void) {

    (void) buffered_write(1, "You ", 4, false);
    (void) buffered_write(1, "like to ", 8, false);
    (void) buffered_write(1, "say ", 4, false);
    (void) buffered_write(1, "Hello world\n", 12, true);
    for (int i = 0; i < 26; i += 1) {
        char b = 'A' + i;
        (void) buffered_write(1, &b, 1, false);
    }
    (void) buffered_write(1, "\n", 1, true);
    return 0;
}
