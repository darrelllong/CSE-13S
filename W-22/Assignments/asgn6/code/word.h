#pragma once

#include "defines.h"

typedef struct {
    Symbol *syms;
    uint32_t length;
} Word;

typedef struct {
    Word *words[MAX_CODE];
} WordTable;

Word *word_create(Symbol *syms, uint32_t length);

Word *word_append_sym(Word *w, Symbol s);

void word_delete(Word **w);

WordTable *wt_create(void);

void wt_reset(WordTable *wt);

void wt_delete(WordTable **wt);
