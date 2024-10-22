#include "lfsr.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>

#define BS 4096

void encryptFile(char *key, uint64_t nonce, int in, int out) {
  char block[BS];
  int  size = 0, length = strlen(key);
  warmLFSR(&nonce);
  while ((size = read(in, block, BS)) > 0) {
    for (int i = 0; i < size; i += 1) {
      block[i] = block[i] ^ key[i % length] ^ randomByte(&nonce); // data ⊕ key ⊕ random
    }
    int written;
    if ((written = write(out, block, size)) < size) {
        fprintf(stderr, "write failed (%d < %d)\n", written, size);
        exit(1);
    }
  }
  return;
}

void (*decryptFile)(char *, uint64_t, int, int) = encryptFile; // They are the same function

#define OPTIONS "k:i:o:n:"

int main(int argc, char **argv) {
    char    *key   = "A default key is a bad idea"; // Default key
    uint64_t nonce = 0x614472726c65216c;            // Default IV

    int in = fileno(stdin), out = fileno(stdout); // Default input and output

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'k':
            key = strdup(optarg); // Freed upon exit
            break;
        case 'i':
            if ((in = open(optarg, O_RDONLY)) < 0) {
                perror(argv[0]); exit(1);
            }
            break;
        case 'o':
            if ((out = open(optarg, O_TRUNC|O_RDWR|O_CREAT, 0644)) < 0) {
                perror(argv[0]); exit(1);
            }
            break;
        case 'n':
            nonce = strtoll(optarg, NULL, 10);
            break;
        }
    }

  encryptFile(key, nonce, in, out);
  close(in);
  close(out);
  return 0;
}
