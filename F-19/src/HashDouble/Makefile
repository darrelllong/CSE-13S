CFLAGS=-Wall -Werror -Wextra -pedantic -O3 -Wshadow
CC=cc
OBJS=hash.o hash.o ../Double/dll.o main.o hashTable.o

main	: $(OBJS)

dll.o	:
	pushd ../Double; make dll.o; popd

infer   :
	make clean; infer-capture -- make; infer-analyze -- make

clean	:
	rm -fr $(OBJS) main infer-out *.dSYM
