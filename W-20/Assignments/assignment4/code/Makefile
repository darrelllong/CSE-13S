EXEC  	= seq

SOURCE 	= $(wildcard *.c)
OBJECT 	= $(SOURCE:%.c=%.o)

CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror -g

.PHONY: all clean infer

all: $(EXEC)

$(EXEC): $(OBJECT)
	$(CC) -o $@ $^ -lm

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(OBJECT) infer-out

infer: clean
	infer-capture -- make
	infer-analyze -- make
