#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t Set;

Set set_empty(void);

bool set_member(Set s, int x);

Set set_insert(Set s, int x);

Set set_remove(Set s, int x);

Set set_union(Set s, Set t);

Set set_intersect(Set s, Set t);

Set set_difference(Set s, Set t);

Set set_complement(Set s);
