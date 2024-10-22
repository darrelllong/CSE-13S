#include <stdio.h>

#include "dll.h"

bool empty(listHead *d) {
  return d->head == NIL && d->tail == NIL;
}

listNode *newNode(char *key) {
  listNode *n = (listNode *)malloc(sizeof(listNode));
  if (n) {
    n->key = strdup(key);
    n->fwd = n->rev = NIL;
  }
  return n;
}

listHead *newList(void) {
  listHead *h = (listHead *)malloc(sizeof(listHead));
  if (h) {
    h->head = h->tail = NIL;
  }
  return h;
}

bool delNode(listNode *n) {
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

bool destroyList(listHead *h) {
  if (h) {
    listNode *p = h->head;
    while (p) {
      listNode *r = p;
      p = p->fwd;
      delNode(r);
    }
    free(h);
    return true;
  } else {
    return false;
  }
}

bool prependList(listHead *h, listNode *n) {
  if (h && n) {
    if (h->head == NIL && h->tail == NIL) {
      h->head = h->tail = n;
    } else {
      n->fwd = h->head;
      h->head->rev = n;
      h->head = n;
      n->rev = NIL;
    }
    return true;
  } else {
    return false;
  }
}

bool appendList(listHead *h, listNode *n) {
  if (h && n) {
    if (h->head == NIL && h->tail == NIL) {
      h->head = h->tail = n;
    } else {
      n->rev = h->tail;
      h->tail->fwd = n;
      h->tail = n;
      n->fwd = NIL;
    }
    return true;
  } else {
    return false;
  }
}

bool insertList(listHead *h, listNode *n) {
  if (h && n) {
    if (h->head == NIL && h->tail == NIL) {
      h->head = h->tail = n;
    } else {
      listNode *p = h->head;
      while (p != NIL && strcmp(n->key, p->key) > 0) {
        p = p->fwd;
      }
      if (p == NIL || p == h->tail) {
        appendList(h, n);
      } else if (p == h->head) {
        prependList(h, n);
      } else {
        n->fwd = p->fwd;
        n->rev = p;
        p->fwd->rev = n;
        p->fwd = n;
      }
    }
    return true;
  } else {
    return false;
  }
}

listNode *popList(listHead *h) {
  if (h && h->head) {
    listNode *p = h->head;
    h->head = p->fwd;
    if (h->head == NIL) {
      h->tail = NIL;
    }
    p->fwd = p->rev = NIL;
    return p;
  }
  return NIL;
}

listNode *dropList(listHead *h) {
  if (h && h->tail) {
    listNode *p = h->tail;
    h->tail = p->rev;
    if (h->tail == NIL) {
      h->head = NIL;
    }
    p->fwd = p->rev = NIL;
    return p;
  }
  return NIL;
}

listNode *findList(listHead *h, char *key) {
  if (h && key) {
    listNode *p = h->head;
    while (p != NIL && strcmp(p->key, key) != 0) {
      p = p->fwd;
    }
    return p;
  }
  return NIL;
}

listNode *findMTF(listHead *h, char *key) {
  listNode *p = findList(h, key);
  if (p) {
    if (p == h->head) {
      return p;
    } else if (p == h->tail) {
      h->tail = p->rev;
      h->tail->fwd = NIL;
      p->fwd = h->head;
      p->rev = NIL;
      h->head->rev = p; // Daniel's fix
      h->head = p;
      return p;
    } else if (p->fwd && p->rev) {
      p->rev->fwd = p->fwd;
      p->fwd->rev = p->rev;
      p->fwd = h->head;
      p->rev = NIL;
      h->head = p;
      return p;
    } else {
      printf("WTF?! %s\n", p->key);
      return NIL;
    }
    return NIL;
  }
  return NIL;
}

void printList(listHead *h) {
  if (h && h->head) {
    listNode *p = h->head;
    while (p != NIL) {
      printf("%s\n", p->key);
      p = p->fwd;
    }
  }
  return;
}
