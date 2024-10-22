#ifndef __SEMWRAPPER_H__
#define __SEMWRAPPER_H__

#include <sys/sem.h>

#ifdef LAME
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};
#endif

//
// Returns the identifier of one initialized semaphore.
//
int sem_create(void);

//
// Removes the specified semaphore from the system.
//
// semid: The semaphore to remove.
//
int sem_delete(int semid);

//
// Increments a semaphore's value by 1.
//
// semid:   The semaphore to increment.
//
int sem_signal(int semid);

//
// Decrements a semaphore's value by 1.
//
// semid:   The semaphore to decrement.
//
int sem_wait(int semid);

#endif
