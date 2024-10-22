EXECBIN  = lrc

SOURCES  = $(wildcard *.c)
HEADERS  = $(wildcard *.h)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

LINKER   = clang
LFLAGS   = -Wall -lm

.PHONY: all clean spotless infer

all: $(EXECBIN) clean

$(EXECBIN): $(OBJECTS)
	$(LINKER) $? $(LFLAGS) -o $@

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

spotless: clean
	rm -rf $(EXECBIN) infer-out

clean:
	rm -rf $(OBJECTS)

infer: spotless
	infer-capture -- make
	infer-analyze -- make

