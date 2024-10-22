#ifndef NIL
#define NIL (void *) 0
#endif

#ifndef MTF
#define MTF true
#endif

#ifndef _LL_DL
#define _LL_DL

#include <stdbool.h>

extern bool moveToFront;

typedef struct listNode listNode;

struct listNode {
  char *key;
  listNode *next;
};

void setMTF(bool);

listNode *newNode(const char *);

void delNode(listNode *);

void delLL(listNode *);

listNode *insertLL(listNode **, const char *);

listNode *findLL(listNode **, const char *);

void printLL(listNode *);

#endif
