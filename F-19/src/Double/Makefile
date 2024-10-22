CFLAGS=-Wall -Werror -Wextra -pedantic -g -Wshadow
CC=clang
OBJS=main.o pop.o drop.o find.o dll.o

main	: dll.o main.c
	$(CC) $(CFLAGS) main.c dll.o -o main

pop	: dll.o pop.c
	$(CC) $(CFLAGS) pop.c dll.o -o pop

drop	: dll.o drop.c
	$(CC) $(CFLAGS) drop.c dll.o -o drop

find	: dll.o find.c
	$(CC) $(CFLAGS) find.c dll.o -o find

infer   :
	make clean; infer-capture -- make; infer-analyze -- make

clean	:
	rm -fr $(OBJS) main infer-out pop drop find a.out *.dSYM
