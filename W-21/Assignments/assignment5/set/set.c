#include "set.h"

Set set_empty(void) {
    return 0x0;
}

Set set_universal(void) {
    return 0xFFFFFFFF;
}

Set set_insert(Set s, uint8_t x) {
    return s | (0x1 << (x & 0x1F));
}

Set set_remove(Set s, uint8_t x) {
    return s & ~(0x1 << (x & 0x1F));
}

bool set_member(Set s, uint8_t x) {
    return (s >> (x & 0x1F)) & 0x1;
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
