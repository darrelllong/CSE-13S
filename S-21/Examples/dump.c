#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

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
