#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

#define MIN_SIZE 16

static inline uint64_t max(uint64_t x, uint64_t y) {
  return (x > y) ? x : y;
}

static inline uint64_t min(uint64_t x, uint64_t y) {
  return (x < y) ? x : y;
}

static inline uint64_t bytes(uint64_t bits) {
  return (bits % 8 == 0) ? bits / 8 : bits / 8 + 1;
}

Set *set_create() {
  Set *s = malloc(sizeof(Set));
  assert(s);

  s->size = MIN_SIZE;
  s->vector = (uint8_t *)malloc(bytes(MIN_SIZE) * sizeof(uint8_t));
  assert(s->vector);

  return s;
}

void set_delete(Set *s) {
  if (s->vector) {
    free(s->vector);
    s->vector = NULL;
  }

  if (s) {
    free(s);
    s = NULL;
  }

  return;
}

void set_insert(Set *s, uint64_t x) {
  if (bytes(x) > bytes(s->size)) {
    uint64_t orig_end = bytes(s->size);
    uint64_t new_end = bytes(x + 1);

    s->size = x + 1;
    s->vector = (uint8_t *)realloc(s->vector, new_end * sizeof(uint8_t));

    // Zero out realloced memory.
    memset(s->vector + orig_end, 0, new_end - orig_end);
  }

  s->vector[x / 8] |= (1 << (x % 8));
  return;
}

void set_remove(Set *s, uint64_t x) {
  s->vector[x / 8] &= ~(1 << (x % 8));
  return;
}

bool set_member(Set *s, uint64_t x) {
  return (s->vector[x / 8] & (1 << (x % 8))) ? true : false;
}

Set *set_union(Set *s, Set *t) {
  Set *u = set_create();
  u->size = max(s->size, t->size);

  // Don't need to zero out since we'll be copying over the bytes.
  u->vector = (uint8_t *)realloc(u->vector, u->size * sizeof(uint8_t));
  assert(u->vector);

  if (s->size > t->size) {
    memcpy(u->vector, s->vector, bytes(s->size));

    for (uint64_t i = 0; i < bytes(t->size); ++i) {
      u->vector[i] |= t->vector[i];
    }
  } else {
    memcpy(u->vector, t->vector, bytes(t->size));

    for (uint64_t i = 0; i < bytes(s->size); ++i) {
      u->vector[i] |= s->vector[i];
    }
  }

  return u;
}

Set *set_intersect(Set *s, Set *t) {
  Set *u = set_create();
  u->size = min(s->size, t->size);

  // Don't need to zero out since we'll be copying over the bytes.
  u->vector = (uint8_t *)realloc(u->vector, u->size * sizeof(uint8_t));
  assert(u->vector);

  if (s->size < t->size) {
    memcpy(u->vector, s->vector, bytes(s->size));

    for (uint64_t i = 0; i < bytes(s->size); ++i) {
      u->vector[i] &= t->vector[i];
    }
  } else {
    memcpy(u->vector, t->vector, bytes(t->size));

    for (uint64_t i = 0; i < bytes(t->size); ++i) {
      u->vector[i] &= s->vector[i];
    }
  }

  return u;
}

Set *set_difference(Set *s, Set *t) {
  Set *u = set_create();
  u->size = s->size;

  // Don't need to zero out since we'll be copying over the bytes.
  u->vector = (uint8_t *)realloc(u->vector, u->size * sizeof(uint8_t));
  assert(u->vector);

  memcpy(u->vector, s->vector, bytes(s->size));

  if (s->size > t->size) {
    for (uint64_t i = 0; i < bytes(t->size); ++i) {
      u->vector[i] &= ~(t->vector[i]);
    }
  } else {
    for (uint64_t i = 0; i < bytes(s->size); ++i) {
      u->vector[i] &= ~(t->vector[i]);
    }
  }

  return u;
}

void set_print(Set *s) {
  printf("{");

  bool first = true;

  for (uint64_t i = 0; i < s->size; ++i) {
    if (set_member(s, i)) {
      if (!first) {
        printf(", ");
      }

      printf("%" PRId64, i);
      first = false;
    }
  }

  printf("}\n");
  return;
}

