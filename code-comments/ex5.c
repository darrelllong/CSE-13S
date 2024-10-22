// BSD License
#include "LICENSE.h"

#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int l, f;
    char b[100];
    f = open("ex0.c", O_RDONLY);
    while ((l = read(f, b, 100)) > 0) {
        write(1, b, l);
    }
    close(f);
    return 0;
}
