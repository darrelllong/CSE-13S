EXECBIN  = newspeak

SOURCES  = $(wildcard *.c)
HEADERS  = $(wildcard *.h)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

.PHONY: all clean infer

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECBIN) $(OBJECTS) infer-out

infer: clean
	infer-capture -- make
	infer-analyze -- make
