#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <assert.h>

int main(void) {
    // Initialize random state.
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_t p, g, a, b, A, B, s, t;
    mpz_inits(p, g, a, b, A, B, s, t, NULL);

    // RFC-3526 (Modular Exponential (MODP) Diffie-Hellman groups).
    // 2048-bit prime p, generator g = 2.
    mpz_set_str(
        p,
        "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
        "29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
        "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
        "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
        "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D"
        "C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F"
        "83655D23DCA3AD961C62F356208552BB9ED529077096966D"
        "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B"
        "E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9"
        "DE2BCBF6955817183995497CEA956AE515D2261898FA0510"
        "15728E5A8AACAA68FFFFFFFFFFFFFFFF",
        16
    );
    mpz_set_ui(g, 2);

    // mpz_urandomb generates uniformly distributed random integer in [0, 2^n).
    mpz_urandomb(a, state, 1024);   // Alice's secret.
    mpz_urandomb(b, state, 1024);   // Bob's secret.
    mpz_powm(A, g, a, p);           // A = g^a mod p
    mpz_powm(B, g, b, p);           // B = g^b mod p
    mpz_powm(s, B, a, p);           // s = B^a mod p (Alice's shared secret)
    mpz_powm(t, A, b, p);           // t = A^b mod p (Bob's shared secret)
    assert(mpz_cmp(s, t) == 0);     // Shared secret should match.

    mpz_clears(p, g, a, b, A, B, s, t, NULL);
    gmp_randclear(state);
    return 0;
}
