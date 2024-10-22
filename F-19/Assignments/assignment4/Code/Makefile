CFLAGS = -Wall -Wextra -Werror -pedantic
CC = clang $(CFLAGS)
OBJECTS = tower.o stack.o 

# .PHONY:	all
# all: tower_stack

# .PHONY:	clean
# clean: rm -f $(OBJECTS) bf

tower_stack		:	tower.o stack.o
		$(CC) -o tower tower.o stack.o 

tower_stack.o	:	tower.c
		$(CC) -c tower.c

stack.o 		:	stack.c
		$(CC) -c stack.c

clean	:
		rm -f $(OBJECTS) tower stack
