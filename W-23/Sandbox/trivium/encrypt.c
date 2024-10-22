#include "trivium.h"
#include <err.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void encryptFile(state *s, FILE *infile, FILE *outfile) {
    size_t bytes;
    union {
        uint64_t word;
        uint8_t bytes[8];
    } buf;

    while ((bytes = fread(buf.bytes, sizeof(uint8_t), 8, infile)) > 0) {
        buf.word ^= randomWord(s);
        if (fwrite(buf.bytes, sizeof(uint8_t), bytes, outfile) != bytes) {
            err(1, "write");
        }
    }
}

int main(void) {
    FILE *infile = stdin;
    FILE *outfile = stdout;

    state s;
    uint128_t key = { 0x614472726c65216c };
    uint128_t nonce = { 0x90128123129dafd };

    startTrivium(&s, key, nonce);
    warmTrivium(&s);
    encryptFile(&s, infile, outfile);

    fclose(infile);
    fclose(outfile);
    return 0;
}
