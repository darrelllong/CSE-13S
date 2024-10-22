#include "lfsr.h"

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BS 4096
#define OPTIONS "k:i:o:n:"

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
        err(1, "write (%d < %d)", written, size);
    }
  }
  return;
}

int main(int argc, char **argv) {
    char    *key   = "A default key is a bad idea"; // Default key
    uint64_t nonce = 0x614472726c65216c;            // Default IV

    int in = STDIN_FILENO, out = STDOUT_FILENO; // Default input and output

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'k':
            key = optarg;
            break;
        case 'i':
            if ((in = open(optarg, O_RDONLY)) < 0) {
                err(1, "%s", optarg);
            }
            break;
        case 'o':
            if ((out = open(optarg, O_WRONLY|O_TRUNC|O_CREAT, 0644)) < 0) {
                err(1, "%s", optarg);
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
