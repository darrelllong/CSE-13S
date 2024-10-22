#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ss.h"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Encrypts data using SS encryption.\n"
        "   Encrypted data is decrypted by the decrypt program.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -h              Display program help and usage.\n"
        "   -v              Display verbose program output.\n"
        "   -i infile       Input file of data to encrypt (default: stdin).\n"
        "   -o outfile      Output file for encrypted data (default: stdout).\n"
        "   -n pbfile       Public key file (default: ss.pub).\n",
        exec);
    return;
}

#define OPTIONS "hvi:o:n:"

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    char *pbfname = "ss.pub";
    FILE *infile = stdin, *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'i': {
            infile = fopen(optarg, "r");
            if (!infile) {
                perror(optarg);
                return EXIT_FAILURE;
            }
            break;
        }
        case 'o': {
            outfile = fopen(optarg, "w"); break;
            if (!outfile) {
                perror(optarg);
                return EXIT_FAILURE;
            }
            break;
        }
        case 'n': pbfname = optarg; break;
        case 'h': usage(argv[0]); return EXIT_SUCCESS;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    FILE *pbfile = fopen(pbfname, "r");
    if (!pbfile) {
        perror(pbfname);
        return EXIT_FAILURE;
    }

    mpz_t n;
    mpz_inits(n, NULL);
    char username[_POSIX_LOGIN_NAME_MAX];

    ss_read_pub(n, username, pbfile);

    if (verbose) {
        fprintf(stderr, "user = %s\n", username);
        gmp_fprintf(stderr, "n (%zu bits) = %Zu\n", mpz_sizeinbase(n, 2), n);
    }

    ss_encrypt_file(infile, outfile, n);
    fclose(infile);
    fclose(outfile);
    fclose(pbfile);
    mpz_clears(n, NULL);
    return EXIT_SUCCESS;
}
