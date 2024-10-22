#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rsa.h"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Decrypts data using RSA decryption.\n"
        "   Encrypted data is encrypted by the encrypt program.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -h              Display program help and usage.\n"
        "   -v              Display verbose program output.\n"
        "   -i infile       Input file of data to decrypt (default: stdin).\n"
        "   -o outfile      Output file for decrypted data (default: stdout).\n"
        "   -n pvfile       Private key file (default: rsa.priv).\n",
        exec);
    return;
}

#define OPTIONS "hvi:o:n:"

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    char *pvfname = "rsa.priv";
    FILE *infile = stdin, *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': usage(argv[0]); return 0;
        case 'v': verbose = true; break;
        case 'i': {
            infile = fopen(optarg, "r"); break;
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
        case 'n': pvfname = optarg; break;
        default: usage(argv[0]); return 1;
        }
    }

    FILE *pvfile = fopen(pvfname, "r");
    if (!pvfile) {
        perror(pvfname);
        return EXIT_FAILURE;
    }

    mpz_t n, d, m, c;
    mpz_inits(n, d, m, c, NULL);

    rsa_read_priv(n, d, pvfile);

    if (verbose) {
        gmp_fprintf(stderr, "n (%zu bits) = %Zu\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr, "d (%zu bits) = %Zu\n", mpz_sizeinbase(d, 2), d);
    }

    rsa_decrypt_file(infile, outfile, n, d);

    fclose(infile);
    fclose(outfile);
    fclose(pvfile);
    mpz_clears(n, d, m, c, NULL);
    return 0;
}
