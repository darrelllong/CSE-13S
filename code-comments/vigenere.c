// BSD License
#include "LICENSE.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ASCII 256

// Name: encode
//
// Function: encode a string using the Vigenère cipher algorithm. Shift a letter
// (byte) forward by the ordinal number of corresponding letter in the key.
//
// Inputs: encryption key (key) and cleartext (cleartext)
//
// Outputs: enciphered text (ciphertext)

void encode(char *key, char *cleartext, char *ciphertext) {
    int i, lk = strlen(key), lc = strlen(cleartext);
    for (i = 0; i < lc; i += 1) {
        ciphertext[i] = (unsigned char) (cleartext[i] + key[i % lk]) % ASCII;
    }
    ciphertext[lc] = '\0';
    return;
}

// Name: decode
//
// Function: decode a string using the Vigenère cipher algorithm. Shift a letter
// (byte) backward by the ordinal number of corresponding letter in the key.
//
// Inputs: decryption key (key) and ciphertext (ciphertext)
//
// Outputs: deciphered text (cleartext)

void decode(char *key, char *ciphertext, char *cleartext) {
    int i, lk = strlen(key), lc = strlen(ciphertext);
    for (i = 0; i < lc; i += 1) {
        cleartext[i] = (unsigned char) (ciphertext[i] - key[i % lk] + ASCII) % ASCII;
    }
    cleartext[lc] = '\0';
    return;
}

void hexprint(char *s) {
    for (int i = 0; i < strlen(s); i += 1) {
        printf("%02x ", (unsigned char) s[i]);
        if ((i + 1) % 20 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    return;
}

#define MAX 1000

int main(void) {
    char key[] = "Frere Jacques, Dormez-vous?";
    char cleartext[MAX], ciphertext[MAX];
    fgets(cleartext, MAX, stdin);
    printf("Key: %s\n", key);
    printf("The given text is:\n%s\n", cleartext);
    encode(key, cleartext, ciphertext);
    printf("The encrypted text is:\n");
    hexprint(ciphertext);
    decode(key, ciphertext, ciphertext);
    printf("\nThe decrypted text is:\n%s\n", ciphertext);
}
