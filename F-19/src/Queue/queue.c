#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

bool emptyQ(queue *q) { return q->fwd == q && q->rev == q; }

queue *newQ(void) {
  queue *q = (queue *) malloc(sizeof(queue));
  if (q) {
    q->key = NIL;        // No key
    q->sentinel = true;  // Acts as the list head
    q->fwd = q->rev = q; // Loops to itself
    return q;
  } else {
    return NIL;
  }
}

void printQ(queue *q, direction d) {
  if (q) {
    switch (d) {
    case FWD:
      for (queue *p = q->fwd; p->sentinel == false; p = p->fwd) {
        printf("%s\n", p->key);
      }
      break;
    case REV:
      for (queue *p = q->rev; p->sentinel == false; p = p->rev) {
        printf("%s\n", p->key);
      }
      break;
    }
    return;
  }
}

queue *newNode(char *s) {
  queue *q = (queue *) malloc(sizeof(queue));
  if (q) {
    q->sentinel = false;    // Regular node
    q->key = strdup(s);     // Copy the string
    q->fwd = q->rev = NIL;  // No connections (yet)
    return q;
  } else {
    return NIL;
  }
}

queue *enqueue(queue *q, queue *n) {
  if (q && n) {
    n->fwd = q->fwd;  // Set successor
    n->rev = q;       // Set predecessor
    q->fwd->rev = n;  // Old head points back
    q->fwd = n;       // Put it at the head
    return n;
  } else {
    return NIL;
  }
}

queue *dequeue(queue *q, direction d) {
  queue *p = NIL;
  if (q) {
    switch (d) {
    case FWD:		// Normal queue
      p = q->fwd;
      if (p == q) {
        return NIL;
      } else {
        q->fwd = p->fwd;
        q->fwd->rev = p->rev;
      }
      break;
    case REV:		// A stack
      p = q->rev;
      if (p == q) {
        return NIL;
      } else {
        q->rev = p->rev;
        q->rev->fwd = p->fwd;
      }
      break;
    }
    if (p) { p->fwd = p->rev = NIL; }
    return p;
  } else {
    return NIL;
  }
}

queue *findQ(queue *q, char *s) {
  for (queue *p = q->fwd; p->sentinel == false; p = p->fwd) {
    if (strcmp(p->key, s) == 0) {
      return p;
    }
  }
  return NIL;
}

queue *pluckQ(queue *q, queue *p) {
  if (q && p) {
    p->rev->fwd = p->fwd; // Predecessor looks forward
    p->fwd->rev = p->rev; // Successor looks backward
    p->fwd = p->rev = NIL;
    return p;
  } else {
    return NIL;
  }
}

queue *findMTF(queue *q, char *s) { return enqueue(q, pluckQ(q, findQ(q, s))); }

bool delNode(queue *n) {
  if (n) {
    if (n->key) {
      free(n->key);
    }
    free(n);
    return true;
  } else {
    return false;
  }
}

bool destroyQ(queue *q) {
  if (q) {
    while (!emptyQ(q)) {
      delNode(dequeue(q, FWD));
    }
    return true;
  } else {
    return false;
  }
}
