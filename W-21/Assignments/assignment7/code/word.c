#include "word.h"
#include "code.h"
#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = (Word *)malloc(sizeof(Word));
    check(w, "Failed to malloc() word.\n");

    w->len = len;
    w->syms = (uint8_t *)malloc(len * sizeof(uint8_t));
    check(w->syms, "Failed to malloc() word byte array.\n");

    memcpy(w->syms, syms, len);
    return w;
}

Word *word_append_sym(Word *w, uint8_t sym) {
    Word *appended = (Word *)malloc(sizeof(Word));
    check(appended, "Failed to malloc() word.\n");

    appended->len = w->len + 1;
    appended->syms = (uint8_t *)malloc(appended->len * sizeof(uint8_t));
    check(appended->syms, "Failed to malloc() word byte array.\n");

    if (w->len) {
        memcpy(appended->syms, w->syms, w->len);
    }

    appended->syms[w->len] = sym;
    return appended;
}

void word_delete(Word *w) {
    if (w->syms) {
        free(w->syms);
        w->syms = NULL;
    }

    free(w);
    return;
}

WordTable *wt_create(void) {
    WordTable *wt = (WordTable *)calloc(MAX_CODE, sizeof(Word *));
    check(wt, "Failed to calloc() word table.\n");

    wt[EMPTY_CODE] = (Word *)calloc(1, sizeof(Word));
    check(wt[EMPTY_CODE], "Failed to calloc() empty word.\n");

    return wt;
}

void wt_reset(WordTable *wt) {
    for (uint32_t i = START_CODE; i < MAX_CODE; i += 1) {
        if (wt[i]) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }

    return;
}

void wt_delete(WordTable *wt) {
    for (uint32_t i = EMPTY_CODE; i < MAX_CODE; i += 1) {
        if (wt[i]) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }

    free(wt);
    return;
}
