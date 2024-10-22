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
#include "randstate.h"
#include "ss.h"

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Generates an SS public/private key pair.\n"
        "\n"
        "USAGE\n"
        "   %s [OPTIONS]\n"
        "\n"
        "OPTIONS\n"
        "   -h              Display program help and usage.\n"
        "   -v              Display verbose program output.\n"
        "   -b bits         Minimum bits needed for public key n (default: 256).\n"
        "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n"
        "   -n pbfile       Public key file (default: ss.pub).\n"
        "   -d pvfile       Private key file (default: ss.priv).\n"
        "   -s seed         Random seed for testing.\n",
        exec);
    return;
}

#define OPTIONS "hvb:i:n:d:s:"
#define BITS    256
#define TSTS    20

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    uint64_t bits = BITS;
    uint64_t seed = (uint64_t)time(NULL);
    uint64_t iterations = TSTS;
    char *pbfname = "ss.pub";
    char *pvfname = "ss.priv";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': usage(argv[0]); return EXIT_SUCCESS;
        case 'v': verbose = true; break;
        case 'b': bits = (uint64_t)strtoull(optarg, NULL, 10); break;
        case 'i': iterations = (uint64_t)strtoull(optarg, NULL, 10); break;
        case 'n': pbfname = optarg; break;
        case 'd': pvfname = optarg; break;
        case 's': seed = (uint64_t) strtoull(optarg, NULL, 10); break;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    FILE *pbfile = fopen(pbfname, "w");
    if (!pbfile) {
        perror(pbfname);
        return EXIT_FAILURE;
    }

    // Private key file should have 0400 permissions (0600 for testing).
    FILE *pvfile = fopen(pvfname, "w");
    if (!pvfile) {
        perror(pvfname);
        return EXIT_FAILURE;
    }
    fchmod(fileno(pvfile), 0600);

    mpz_t p, q, n, pq, d;
    mpz_inits(p, q, n, pq, d, NULL);

    randstate_init(seed);
    srandom(seed);
    ss_make_pub(p, q, n, bits, iterations);
    ss_make_priv(d, pq, p, q);

    char *username = getenv("USER");
    ss_write_pub(n, username, pbfile);
    ss_write_priv(pq, d, pvfile);

    if (verbose) {
        fprintf(stderr, "user = %s\n", username);
        gmp_fprintf(stderr, "p  (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_fprintf(stderr, "q  (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_fprintf(stderr, "n  (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr, "pq (%zu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_fprintf(stderr, "d  (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(p, q, n, pq, d, NULL);
    return EXIT_SUCCESS;
}
