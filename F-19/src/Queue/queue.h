#ifndef _Q_DL
#define _Q_DL
#ifndef NIL
#define NIL (void *) 0
#endif

#include <stdbool.h>

typedef enum {FWD, REV} direction;

typedef struct queue queue;

struct queue {
  bool sentinel;
  char *key;
  queue *fwd, *rev;
};

bool emptyQ(queue *);

queue *newQ(void);

void printQ(queue *, direction d);

queue *newNode(char *);

bool delNode(queue *);

bool destroyQ(queue *);

queue *enqueue(queue *, queue *);

queue *dequeue(queue *, direction);

queue *findQ(queue *, char *);

queue *pluckQ(queue *, queue *);

queue *findMTF(queue *, char *);

#endif
