#include <assert.h>
#include <errno.h>
#include <limits.h>
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
            "   Encrypts data using RSA encryption.\n"
            "   Encrypted data is decrypted by the decrypt program.\n"
            "\n"
            "USAGE\n"
            "   %s [-hv] [-i infile] [-o outfile] -n pubkey -d privkey\n"
            "\n"
            "OPTIONS\n"
            "   -h              Display program help and usage.\n"
            "   -v              Display verbose program output.\n"
            "   -i infile       Input file of data to encrypt (default: stdin).\n"
            "   -o outfile      Output file for encrypted data (default: stdout).\n"
            "   -n pbfile       Public key file (default: rsa.pub).\n",
            exec);
    return;
}

#define OPTIONS "hvi:o:n:"

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    char *pbfname = "rsa.pub";
    FILE *infile = stdin, *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;
        case 'i':
            infile = fopen(optarg, "r");
            break;
        case 'o':
            outfile = fopen(optarg, "w");
            break;
        case 'd':
            pbfname = optarg;
            break;
        case 'h':
            usage(argv[0]);
            return EXIT_SUCCESS;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    FILE *pbfile = fopen(pbfname, "r");
    if (!pbfile) {
        fprintf(stderr, "%s: %s\n", pbfname, strerror(errno));
        return EXIT_FAILURE;
    }

    mpz_t n, e, s, m, c;
    mpz_inits(n, e, s, m, c, NULL);
    char username[_POSIX_LOGIN_NAME_MAX];

    rsa_read_pub(n, e, s, username, pbfile);

    if (verbose) {
        fprintf(stderr, "user = %s\n", username);
        gmp_fprintf(stderr, "s (%zu bits) = %Zu\n", mpz_sizeinbase(s, 2), s);
        gmp_fprintf(stderr, "n (%zu bits) = %Zu\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr, "e (%zu bits) = %Zu\n", mpz_sizeinbase(e, 2), e);
    }

    // Compute expected value of signature: the public key's listed username as mpz_t.
    mpz_set_str(m, username, 62);

    // Encrypt file if and only if the public key is verified.
    int status = EXIT_SUCCESS;
    if (rsa_verify(m, s, e, n)) {
        rsa_encrypt_file(infile, outfile, n, e);
    } else {
        fprintf(stderr, "Error: invalid key.\n");
        status = EXIT_FAILURE;
    }

    fclose(infile);
    fclose(outfile);
    fclose(pbfile);
    mpz_clears(n, e, s, m, c, NULL);
    return status;
}
