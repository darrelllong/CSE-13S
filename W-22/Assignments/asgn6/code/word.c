#include "word.h"
#include <stdlib.h>
#include <string.h>

Word *word_create(Symbol *syms, uint32_t length) {
    Word *w = (Word *)malloc(sizeof(Word));

    w->length = length;
    if (w->length > 0) {
        w->syms = (Symbol *)malloc(length * sizeof(Symbol));
        memcpy(w->syms, syms, length);
    } else {
        w->syms = (Symbol *)malloc(sizeof(Symbol));
    }

    return w;
}

Word *word_append_sym(Word *w, Symbol s) {
    Word *appended = (Word *)malloc(sizeof(Word));

    appended->length = w->length + 1;
    appended->syms = (Symbol *)malloc(appended->length * sizeof(Symbol));

    if (w->length) {
        memcpy(appended->syms, w->syms, w->length);
    }

    appended->syms[w->length] = s;
    return appended;
}

void word_delete(Word **w) {
    if (*w) {
        if ((*w)->syms) {
            free((*w)->syms);
            (*w)->syms = NULL;
        }
        free(*w);
        *w = NULL;
    }
}

uint32_t word_length(Word *w) {
    return w->length;
}

WordTable *wt_create(void) {
    WordTable *wt = (WordTable *)calloc(1, sizeof(WordTable));
    wt->words[EMPTY_CODE] = word_create(NULL, 0);
    return wt;
}

void wt_reset(WordTable *wt) {
    for (int i = START_CODE; i < MAX_CODE; i += 1) {
        if (wt->words[i]) {
            word_delete(&wt->words[i]);
        }
    }
}

void wt_delete(WordTable **wt) {
    if (*wt) {
        for (int i = EMPTY_CODE; i < MAX_CODE; i += 1) {
            if ((*wt)->words[i]) {
                word_delete(&(*wt)->words[i]);
            }
        }
        free(*wt);
        *wt = NULL;
    }
}
