CFLAGS=-Wall -Werror -Wextra -pedantic -O3 -Wshadow
CC=cc
OBJS=hash.o hash.o ll.o main.o hashTable.o

main	: $(OBJS)

infer   :
	make clean; infer-capture -- make; infer-analyze -- make

clean	:
	rm -fr $(OBJS) main infer-out
