#include "ss.h"

#include "numtheory.h"

#include <stdlib.h>

#define TOSTRING(X) TOSTRING_HELPER(X)
#define TOSTRING_HELPER(X) #X

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    // 20-40% of bits go to p
    uint64_t pbits = random() % (nbits / 5) + nbits / 5;
    uint64_t qbits = nbits - 2 * pbits;

    // gen primes
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);

    // set n
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    // Check q | p - 1
    bool bad = false;
    mpz_t var;
    mpz_init(var);
    mpz_sub_ui(var, p, 1);      // var = p - 1
    mpz_tdiv_r(var, var, q);    // var = var % q
    if (mpz_cmp_ui(var, 0) == 0) {
        bad = true;
    }

    // Check if p | q - 1
    mpz_sub_ui(var, q, 1);
    mpz_tdiv_r(var, var, p);
    if (mpz_cmp_ui(var, 0) == 0) {
        bad = true;
    }
    mpz_clear(var);

    // Regenerate keys if previous assertion failed
    if (bad) {
        ss_make_pub(p, q, n, nbits, iters);
    }
}

void ss_make_priv(mpz_t d, mpz_t pq, mpz_t p, mpz_t q) {
    /*
        pm1 = p - 1;
        qm1 = q - 1;
        gcd = gcd (pm1, qm1);
        lcm = pm1 * qm1 / gcd;
        n = p^2*q
        d = mod_inverse(n, lcm)
        pq = p * q
    */
    mpz_t pm1, qm1, gcd_v, lcm, n;
    mpz_inits(pm1, qm1, gcd_v, lcm, n, NULL);

    mpz_sub_ui(pm1, p, 1);
    mpz_sub_ui(qm1, q, 1);

    gcd(gcd_v, pm1, qm1);
    mpz_mul(lcm, pm1, qm1);
    mpz_divexact(lcm, lcm, gcd_v);

    mpz_mul(n, p,  p);
    mpz_mul(n, n, q);

    mod_inverse(d, n, lcm);

    mpz_mul(pq, p, q);

    mpz_clears(pm1, qm1, gcd_v, lcm, n, NULL);
}

void ss_write_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_write_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%" TOSTRING(_POSIX_LOGIN_NAME_MAX) "s\n", n, username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_encrypt(mpz_t c, mpz_t m, mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, mpz_t n) {
    /*
     * 1. alloc buffer of size lg(sqrt(n)) / 8 bytes
     *      - the data you import must be <pq, ensure this by
     *        underestimating pq as sqrt(n) which is p*sqrt(q)
     *
     * 2. pad first byte of buffer with 0xff
     *      - protects against the event that buffer exports
     *        to 0 or 1 as 0^n%n == 0 and 1^n%n == 1 aka m == c
     *
     * 3. encrypt data until EOF
     *      (a) read in buffer size - 1 bytes -- avoid overwriting 0xff
     *      (b) convert data to mpz_t message and encrypt as cipher
     *      (c) write cipher to output file
     */

    mpz_t m, c;
    mpz_inits(m, c, NULL);

    // Prepare Block s.t. it can never be >=pq
    size_t const BLOCK_SIZE = (mpz_sizeinbase(n, 2) / 2 - 1) / 8;
    uint8_t* buffer = malloc(BLOCK_SIZE);
    buffer[0] = 0xff;

    // Encrypt data in blocks
    size_t red;
    while ((red = fread(buffer + 1, 1, BLOCK_SIZE - 1, infile))) {
        mpz_import(m, red + 1, 1, 1, 1, 0, buffer);
        ss_encrypt(c, m, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }

    free(buffer); buffer = NULL;
    mpz_clears(m, c, NULL);
}

void ss_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, mpz_t d, mpz_t pq) {
    /*
     * 1. alloc buffer minimum size lg(sqrt(n)) / 8 bytes
     *      - we don't know n in this context so overestimate w/ pq
     *        sqrt(n) = p * sqrt(q) < pq
     *
     * 2. decrypt data until EOF
     *      (a) scan 1 hex line as mpz_t cipher
     *      (b) decrypt cipher to message
     *      (c) export message to buffer
     *      (d) write buffer to outfile w/o 0xff pad
     */
    mpz_t c, m;
    mpz_inits(c, m, NULL);

    // Prepare Block s.t. it can hold digits with lg(n)/2 bits
    size_t const BLOCK_SIZE = mpz_sizeinbase(pq, 2) / 8;
    uint8_t* buffer = malloc(BLOCK_SIZE);

    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        ss_decrypt(m, c, d, pq);
        size_t bytes;
        mpz_export(buffer, &bytes, 1, 1, 1, 0, m);
        fwrite(buffer + 1, 1, bytes - 1, outfile);
    }
    free(buffer); buffer = NULL;
    mpz_clears(c, m, NULL);
}
