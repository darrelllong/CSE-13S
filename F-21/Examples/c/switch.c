#include "set.h"

#include <inttypes.h>
#include <stdio.h>

void foo() {
    puts("Foo!");
}

void bar() {
    puts("Bar!");
}

void bleh() {
    puts("bleh!");
}

typedef enum { FOO, BAR, MOO, WAH, BAZ } dude;

int main(void) {
    void (*yak[])() = { foo, bar, bleh, bleh, bleh };
    Set s = 0;
    s = set_insert(s, BAR);
    for (dude i = FOO; i <= BAZ; i += 1) {
        if (set_member(s, i)) {
            yak[i]();
        }
    }
    return 0;
}
