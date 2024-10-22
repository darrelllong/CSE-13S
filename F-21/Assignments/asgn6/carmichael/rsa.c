#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t pbits = (random() % (nbits / 2)) + (nbits / 4); // pbits ∊ [n/4, ..., 3n/4]
    uint64_t qbits = nbits - pbits;

    make_prime(p, pbits, iters); // Choose our priv primes p & q
    make_prime(q, qbits, iters);

    mpz_t d, carmichael, tP, tQ, tE;
    mpz_inits(d, carmichael, tP, tQ, tE, NULL);
    mpz_sub_ui(tP, p, 1);
    mpz_sub_ui(tQ, q, 1);

    mpz_mul(n, p, q); // n = p × q
    lcm(carmichael, tP, tQ); // λ = LCM(p - 1, q - 1)

    do {
        mpz_urandomb(tE, state, nbits);
        gcd(d, tE, carmichael);
    } while (mpz_cmp_ui(d, 1)); // gcd(tE, 𝜑(n)) = 1

    mpz_set(e, tE); // We have found our e
    mpz_clears(d, carmichael, tP, tQ, tE, NULL); // Free local variables
    return;
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
    return;
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
    return;
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t totient, tP, tQ;
    mpz_inits(totient, tP, tQ, NULL);
    mpz_sub_ui(tP, p, 1);
    mpz_sub_ui(tQ, q, 1);
    mpz_mul(totient, tP, tQ);  // 𝜑(n) = (p - 1)(q - 1)
    mod_inverse(d, e, totient); // e * d ≡ 1 (mod 𝜑(n))
    mpz_clears(totient, tP, tQ, NULL);
    return;
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    return;
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
    return;
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n); // c = m**e (mod n)
    return;
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);

    // Encrypt input in small blocks, where the block is always prepended with 0xFF.
    // This is due to the possibility of having blocks containing all zeroes.
    // Having the 0xFF at the beginning ensures proper mpz import/export.
    size_t nbytes = 0;
    size_t blocksize = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = calloc(blocksize, sizeof(uint8_t));
    block[0] = 0xFF; // 0th byte is the prepended 0xFF.

    while (!feof(infile)) {
        // Read into block buffer after 0th byte, since the 0th byte is the 0xFF.
        // The original block size includes the 0xFF, so we read one less.
        if ((nbytes = fread(block + 1, sizeof(uint8_t), blocksize - 1, infile)) > 0) {
            // Convert bytes read + 0xFF byte into message to encrypt.
            mpz_import(m, nbytes + 1, 1, sizeof(uint8_t), 0, 0, block);
            rsa_encrypt(c, m, e, n);
            gmp_fprintf(outfile, "%Zx\n", c);
        }
    }

    mpz_clears(m, c, NULL);
    free(block);
    return;
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);

    // Decrypt input in small blocks.
    // Note: the encrypt program always prepends blocks with 0xFF.
    // This is due to the possibility of having blocks containing all zeroes.
    // Having the 0xFF at the beginning ensures proper mpz import/export.
    size_t nbytes = 0;
    size_t blocksize = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = calloc(blocksize, sizeof(uint8_t));

    while (!feof(infile)) {
        if (gmp_fscanf(infile, "%Zx\n", c) > 0) {
            rsa_decrypt(m, c, d, n);
            // Convert decrypted message back into bytes.
            mpz_export(block, &nbytes, 1, sizeof(uint8_t), 0, 0, m);

            // Write after 0th byte, since that must be the prepended 0xFF.
            // This means we also write one less than the number of bytes that were exported.
            fwrite(block + 1, sizeof(uint8_t), nbytes - 1, outfile);
        }
    }

    mpz_clears(m, c, NULL);
    free(block);
    return;
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n); // m = c**d (mod n)
    return;
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n); // s = m**d (mod n)
    return;
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t tmpM;
    mpz_init(tmpM); // Temporary to avoid modifying original signature.
    pow_mod(tmpM, s, e, n); // m = s**e (mod n) if the signature is valid
    bool verified = mpz_cmp(m, tmpM) == 0; // Need to save result to clear temporary.
    mpz_clear(tmpM);
    return verified;
}
