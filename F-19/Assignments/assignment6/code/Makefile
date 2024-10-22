OLD=true
MASK=0x00ffffff
CFLAGS=-Wall -Werror -Wextra -pedantic
CC=clang
OBJS=sorting.o bubblesort.o minsort.o insertionsort.o quicksort.o \
shakersort.o mergesort.o shellsort.o heapsort.o stack.o

sorting	: Makefile $(OBJS)
	$(CC) -o sorting $(OBJS)

sorting.o	:	sorting.c
	$(CC) $(CFLAGS) -DOLD=$(OLD) -DMASK=$(MASK) -c sorting.c

clean	:
	rm -f $(OBJS) sorting
infer	:
	make clean; infer-capture -- make; infer-analyze -- make
