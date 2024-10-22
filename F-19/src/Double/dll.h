#ifndef _DLL_DL
#define _DLL_DL
#ifndef NIL
#define NIL (void *)0
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct listNode listNode;

struct listNode {
  char *key;
  listNode *fwd, *rev;
};

typedef struct {
  listNode *head, *tail;
} listHead;

bool empty(listHead *);

listNode *newNode(char *);

listHead *newList(void);

bool delNode(listNode *);

bool destroyList(listHead *);

bool prependList(listHead *, listNode *);

bool appendList(listHead *, listNode *);

bool insertList(listHead *, listNode *);

listNode *popList(listHead *);

listNode *dropList(listHead *);

listNode *findList(listHead *, char *);

listNode *findMTF(listHead *, char *);

void printList(listHead *);

#endif
