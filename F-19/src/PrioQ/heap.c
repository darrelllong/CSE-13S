// heap.c
//
// DL
// Mon Feb  1 18:49:55 PST 1993

#include "heap.h"
#include <inttypes.h>
#include <stdbool.h>

void new_heap(heap *h) {
  h->base = (node *)malloc(sizeof(node));
  h->count = 0;
  h->allocation = 1;
}

bool empty(heap *h) { return h->count == 0; }

node top(heap *h) { return h->base[0]; }

// Adapted from Sara Baase's "Computer Algorithms".

void fix_heap(heap *h, uint32_t root, node n, uint32_t bound) {

  uint32_t vacant, smallest;

  // Note that we have to work starting with 1 to make the arithmetic work, even
  // though arrays in C start at 0.

  vacant = root;

  while (2 * vacant <= bound) {

    // Find the left child of the vacant slot.

    smallest = 2 * vacant;

    // Is it the smallest of the two?  If not, choose the right child.

    if ((smallest < bound) &&
        (h->base[smallest].time < h->base[smallest - 1].time)) {
      smallest = smallest + 1;
    }

    // Lighter elements percolate up.

    if (n.time > h->base[smallest - 1].time) {
      h->base[vacant - 1] = h->base[smallest - 1];
      vacant = smallest;
    } else {
      break;
    }
  }

  // While the new element trickles down to its proper place.

  h->base[vacant - 1] = n;
}

node dequeue(heap *h) {

  // We always delete the root.

  node n = top(h), r = h->base[h->count - 1];

  // Right-most child replaces the root, which will then trickle down.

  h->count -= 1; fix_heap(h, 1, r, h->count);

  return n;
}

void enqueue(heap *h, node n) {

  uint32_t runner;

  // Allen Van Gelder's solution: grow the array if we've run out of space.
  // We'll pay at most O(n) extra copies.

  if (h->count == h->allocation) {
    h->allocation *= 2;
    h->base = (node *)realloc(h->base, h->allocation * sizeof(node));
  }

  // Put the new node at the far end of the heap.

  h->base[h->count] = n; h->count += 1;

  // And then let it percolate up to its proper place.

  runner = h->count;

  while (runner > 1) {
    fix_heap(h, runner / 2, h->base[runner / 2 - 1], h->count);
    runner /= 2;
  }
}
