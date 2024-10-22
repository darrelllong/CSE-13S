// BSD License
#include "LICENSE.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <unistd.h>

#define STDIN  0
#define STDOUT 1
#define STDERR 2

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    int count;
    unsigned char buffer[BUFFER_SIZE];

    if (argc < 2) {
        while ((count = read(STDIN, buffer, BUFFER_SIZE)) > 0)
            write(STDOUT, buffer, count);
    } else {
        int i;
        for (i = 1; i < argc; i++) {
            int file;
            file = open(argv[i], O_RDONLY);
            if (file < 0) {
                perror("cat");
                exit(errno);
            }
            while ((count = read(file, buffer, BUFFER_SIZE)) > 0)
                write(STDOUT, buffer, count);
            close(file);
        }
    }
    exit(0);
}
