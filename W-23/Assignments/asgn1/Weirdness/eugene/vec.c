#include "vec.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_BUF_SIZE 16

Vec vec_new(void) {
    Vec vec;
    vec.len = 0;
    vec.cap = 0;
    vec.buf = NULL;
    return vec;
}

void vec_drop(Vec *vec) {
    if (vec) {
        free(vec->buf);
        vec->buf = NULL;
    }
}

size_t vec_len(Vec *vec) {
    return vec->len;
}

uint32_t *vec_buf(Vec *vec) {
    return vec->buf;
}

void vec_reserve(Vec *vec, size_t n) {
    if (!vec->buf) {
        vec->cap = DEFAULT_BUF_SIZE;
    }

    size_t cap = vec->cap;
    size_t needed = vec->len + n;
    while (vec->cap < needed) {
        vec->cap *= 2;
    }

    if (vec->cap == DEFAULT_BUF_SIZE || cap < vec->cap) {
        vec->buf = realloc(vec->buf, vec->cap * sizeof(*vec->buf));
    }
}

void vec_push(Vec *vec, uint32_t x) {
    vec_reserve(vec, 1);
    vec->buf[vec->len++] = x;
}

uint32_t vec_pop(Vec *vec) {
    return vec->len == 0 ? -1 : vec->buf[--vec->len];
}

void vec_clear(Vec *vec) {
    vec->len = 0;
}

void vec_print(Vec *vec) {
    printf("[");
    for (size_t i = 0; i < vec->len; i += 1) {
        printf("%" PRIu32, vec->buf[i]);
        if (i + 1 != vec->len) {
            printf(", ");
        }
    }
    printf("]\n");
}
