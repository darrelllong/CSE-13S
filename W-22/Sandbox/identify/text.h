#pragma once

#include <stdio.h>
#include <stdint.h>

extern uint32_t noiselimit;

typedef struct Text Text;

typedef enum { EUCLIDEAN, MANHATTAN, COSINE } Metric;

Text *text_create(char *author, FILE *infile, Text *noise);

void text_delete(Text **text);

char *text_author(Text *text);

double text_dist(Text *text1, Text *text2, Metric metric);

void text_print(Text *text);
