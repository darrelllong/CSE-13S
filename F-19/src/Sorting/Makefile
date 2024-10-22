MASK=0x00ffffff
CFLAGS=-Wall -Werror -Wextra -pedantic -O3
CC=cc
OBJS=sorting.o bubblesort.o minsort.o insertionsort.o quicksort.o \
mergesort.o shellsort.o heapsort.o stack.o binsert.o shakersort.o semwrapper.o

sorting	: Makefile $(OBJS)
	$(CC) -o sorting $(OBJS)

sorting.o	:	sorting.c
	$(CC) $(CFLAGS) -DMASK=$(MASK) -c sorting.c

clean	:
	rm -rf $(OBJS) sorting infer-out
infer	:
	make clean; infer-capture -- make; infer-analyze -- make
