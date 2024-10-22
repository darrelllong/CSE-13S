#include "ll.h"
#include <assert.h>
#include <string.h>

int main(void) {
    LinkedList *l = ll_create(true); // Enable move_to_front.

    ll_insert(l, "didgeridoo", "instrument");
    ll_insert(l, "violin", "instrument");
    ll_insert(l, "cello", "instrument");
    ll_insert(l, "piano", NULL);
    ll_print(l);

    assert(ll_length(l) == 4);
    ll_print(l);

    Node *n = ll_lookup(l, "violin");
    assert(n);
    assert(strcmp(n->oldspeak, "violin") == 0);
    node_print(n);

    n = ll_lookup(l, "piano");
    assert(n);
    assert(!n->newspeak);
    ll_print(l);

    n = ll_lookup(l, "didgeridoo");
    assert(n);
    ll_print(l);

    assert(!ll_lookup(l, "quirky"));

    ll_delete(&l);
    assert(!l);

    return 0;
}
