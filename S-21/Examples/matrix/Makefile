EXEC = main

SOURCES = $(wildcard *.c)
OBJECTS = matrix.o main.o

CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all : $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) -c $(SOURCES) $(CFLAGS)

clean : 
	rm -rf $(OBJECTS) $(EXEC)