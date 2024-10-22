#include "set.h"

Set set_empty(void) {
    return 0;
}

bool set_member(Set s, int x) {
    return s & (1 << (x % 32));
}

Set set_insert(Set s, int x) {
    return s | (1 << (x % 32));
}

Set set_remove(Set s, int x) {
    return s & ~(1 << (x % 32));
}

Set set_union(Set s, Set t) {
    return s | t;
}

Set set_intersect(Set s, Set t) {
    return s & t;
}

Set set_difference(Set s, Set t) {
    return s & ~t;
}

Set set_complement(Set s) {
    return ~s;
}
