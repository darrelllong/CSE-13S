EXEC  	= sorting

SOURCE 	= $(wildcard *.c)
OBJECT 	= $(SOURCE:%.c=%.o)

CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror

UNAME 	= $(shell uname -s)

ifeq ($(UNAME), Linux)
	CFLAGS += -DLAME
endif

.PHONY: all clean infer

all: $(EXEC)

$(EXEC): $(OBJECT)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(OBJECT) infer-out

infer: spotless
	infer-capture -- make
	infer-analyze -- make
