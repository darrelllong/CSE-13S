EXECBIN  = lrc

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra
LFLAGS   = -Wall

.PHONY: all clean infer

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) $(LFLAGS) $^ -o $@

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECBIN) infer-out

format:
	clang-format -i -style=file *.c

infer: clean
	infer-capture -- make
	infer-analyze -- make

