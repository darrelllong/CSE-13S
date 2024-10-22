CFLAGS=-Wall -Werror -Wextra -pedantic -O3 -Wshadow -pg
CC=cc
OBJS=prng.o cntr.o full.o

all	: prng cntr full

prng	: $(OBJS)
	cc -o prng prng.o

cntr	: $(OBJS)
	cc -o cntr cntr.o

full	: $(OBJS)
	cc -o full full.o

infer   :
	make clean; infer-capture -- make; infer-analyze -- make

gprof   :
	make clean; make; cc -pg -o prng prng.c; ./prng; gprof prng

clean	:
	rm -fr $(OBJS) prng cntr full infer-out *.dSYM
