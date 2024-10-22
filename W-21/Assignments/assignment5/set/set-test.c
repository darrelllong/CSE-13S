#include "set.h"
#include <assert.h>

typedef enum Dogs { Snoopy, Scooby, Pluto, Odie, Clifford } Dogs;

int main(void) {
    // All the dogs should be in the set.
    Set s = set_universal();
    for (Dogs d = Snoopy; d <= Clifford; d += 1) {
        assert(set_member(s, d));
    }

    // Snoopy and Scooby shouldn't be in the set.
    s = set_remove(s, Snoopy); assert(!set_member(s, Snoopy));
    s = set_remove(s, Scooby); assert(!set_member(s, Scooby));

    // Pluto and Odie should be in the set.
    s = set_empty();
    s = set_insert(s, Pluto); assert(set_member(s, Pluto));
    s = set_insert(s, Odie); assert(set_member(s, Odie));

    // Add Snoopy, Pluto, and Clifford to t.
    s = set_universal();
    Set t = set_empty();
    for (Dogs d = Snoopy; d <= Clifford; d += 2) {
        t = set_insert(t, d);
    }

    // The only dogs in u should be Scooby and Odie (odd-indexed).
    Set u = set_difference(s, t);
    for (Dogs d = Snoopy; d <= Clifford; d += 1) {
        if (d & 1) {
            assert(set_member(u, d));
        } else {
            assert(!set_member(u, d));
        }
    }

    // t union u should yield the universal set.
    Set v = set_union(t, u);
    for (Dogs d = Snoopy; d <= Clifford; d += 1) {
        assert(set_member(v, d));
    }

    // Universal set intersected with u should just be u itself.
    Set w = set_intersect(s, u);
    for (Dogs d = Snoopy; d <= Clifford; d += 1) {
        if (d & 1) {
            assert(set_member(w, d));
        } else {
            assert(!set_member(w, d));
        }
    }

    // Make sure we get 32 elements.
    Set x = set_universal();
    for (int i = 0; i < 32; i += 1) {
        assert(set_member(x, i));
    }

    return 0;
}
