#ifndef __SET_H__
#define __SET_H__

#include <inttypes.h>
#include <stdbool.h>

typedef struct Set {
  uint8_t *vector;
  uint64_t size;
} Set;

Set *set_create(void);

void set_delete(Set *s);

void set_insert(Set *s, uint64_t x);

void set_remove(Set *s, uint64_t x);

bool set_member(Set *s, uint64_t x);

Set *set_union(Set *s, Set *t);

Set *set_intersect(Set *s, Set *t);

Set *set_difference(Set *s, Set *t);

void set_print(Set *s);

#endif
