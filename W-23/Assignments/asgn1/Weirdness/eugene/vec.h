#pragma once

#include <stdint.h>
#include <sys/types.h>

typedef struct {
    size_t len;
    size_t cap;
    uint32_t *buf;
} Vec;

Vec vec_new(void);

void vec_drop(Vec *vec);

size_t vec_len(Vec *vec);

uint32_t *vec_buf(Vec *vec);

void vec_reserve(Vec *vec, size_t n);

void vec_push(Vec *vec, uint32_t x);

uint32_t vec_pop(Vec *vec);

void vec_clear(Vec *vec);

void vec_print(Vec *vec);
