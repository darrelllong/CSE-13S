#include "parser.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#define WORD "[a-zA-Z0-9_]+(('|-)[a-zA-Z0-9_]+)*"

int main(int argc, char **argv) {
  FILE *infile = stdin;

  if (argc == 2) {
    if (!(infile = fopen(argv[1], "r"))) {
      perror("fopen");
      exit(1);
    }
  }

  regex_t re;

  if (regcomp(&re, WORD, REG_EXTENDED)) {
    perror("regcomp");
    exit(1);
  }

  char *word = NULL;

  while ((word = next_word(infile, &re))) {
    printf("Word: %s\n", word);
  }

  fclose(infile);
  clear_words();
  regfree(&re);
  return 0;
}
