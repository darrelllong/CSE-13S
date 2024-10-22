#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "rsa.h"

void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "   Generates an RSA public/private key pair.\n"
            "\n"
            "USAGE\n"
            "   %s [-hv] [-b bits] -n pbfile -d pvfile\n"
            "\n"
            "OPTIONS\n"
            "   -h              Display program help and usage.\n"
            "   -v              Display verbose program output.\n"
            "   -b bits         Minimum bits needed for public key n.\n"
            "   -c confidence   Miller-Rabin iterations for testing primes (default: 50).\n"
            "   -n pbfile       Public key file (default: rsa.pub).\n"
            "   -d pvfile       Private key file (default: rsa.priv).\n"
            "   -s seed         Random seed for testing.\n",
            exec);
    return;
}

#define OPTIONS "hvb:c:n:d:s:"
#define BITS    256
#define TSTS    20

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    uint64_t bits = BITS;
    uint64_t seed = (uint64_t)time(NULL);
    uint64_t confidence = TSTS;
    char *pbfname = "rsa.pub";
    char *pvfname = "rsa.priv";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            usage(argv[0]);
            return EXIT_SUCCESS;
        case 'v':
            verbose = true;
            break;
        case 'b':
            bits = (uint64_t)strtoull(optarg, NULL, 10);
            break;
        case 'c':
            confidence = (uint64_t)strtoull(optarg, NULL, 10);
            break;
        case 'n':
            pbfname = optarg;
            break;
        case 'd':
            pvfname = optarg;
            break;
        case 's':
            seed = (uint64_t)strtoull(optarg, NULL, 10);
            break;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    FILE *pbfile = fopen(pbfname, "w");
    if (!pbfile) {
        fprintf(stderr, "%s: %s\n", pbfname, strerror(errno));
        return EXIT_FAILURE;
    }

    // Private key file should have 0400 permissions (0600 for testing).
    FILE *pvfile = fopen(pvfname, "w");
    if (!pvfile) {
        fprintf(stderr, "%s: %s\n", pvfname, strerror(errno));
        return EXIT_FAILURE;
    }
    fchmod(fileno(pvfile), 0600);

    mpz_t p, q, n, e, d, s;
    mpz_inits(p, q, n, e, d, s, NULL);

    rsaInitialize(seed);
    rsaMakePublic(p, q, n, e, bits, confidence);
    rsaMakePrivate(d, e, p, q);

    // Compute signature using username.
    char *username = getenv("USER");
    mpz_set_str(s, username, 62);
    rsaSign(s, s, d, n);

    rsaWritePublic(n, e, s, username, pbfile);
    rsaWritePrivate(n, d, pvfile);

    if (verbose) {
        fprintf(stderr, "user = %s\n", username);
        gmp_fprintf(stderr, "s (%zu bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_fprintf(stderr, "p (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_fprintf(stderr, "q (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_fprintf(stderr, "n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr, "e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_fprintf(stderr, "d (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    fclose(pbfile);
    fclose(pvfile);
    mpz_clears(p, q, n, e, d, s, NULL);
    return EXIT_SUCCESS;
}
