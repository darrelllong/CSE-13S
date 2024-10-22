EXECBIN  = parser-test

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

.PHONY: all debug clean infer

all: $(EXECBIN)

debug: CFLAGS += -DDEBUG
debug: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECBIN) $(OBJECTS) infer-out

infer: clean
	infer-capture -- make
	infer-analyze -- make
