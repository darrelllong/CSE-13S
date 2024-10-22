#include "hamming.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "i:o:"

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': { infile = fopen(optarg, "rb"); break; }
        case 'o': { outfile = fopen(optarg, "wb"); break; }
        default:  { return 1; }
        }
    }

    struct stat statbuf;
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    int c = 0;
    while ((c = fgetc(infile)) != EOF) {
        fputc(encode(c & 0x0F), outfile);
        fputc(encode((c & 0xF0) >> 4), outfile);
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}
