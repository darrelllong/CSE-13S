CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: pig

pig: pig.o
	$(CC) -o pig pig.o

pig.o: pig.c names.h
	$(CC) $(CFLAGS) -c pig.c

clean:
	rm -f pig pig.o

format:
	clang-format -i -style=file *.{c,h}
