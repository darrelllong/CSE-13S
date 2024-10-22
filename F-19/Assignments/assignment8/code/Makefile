EXECBIN  = lzwcoder

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

.PHONY: all debug clean spotless infer

all: $(EXECBIN)

debug: CFLAGS += -DDEBUG
debug: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^
	rm -f encode; ln $(EXECBIN) encode
	rm -f decode; ln $(EXECBIN) decode

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

spotless: clean
	rm -rf $(EXECBIN) encode decode infer-out

clean:
	rm -rf $(OBJECTS)

infer: spotless
	infer-capture -- make
	infer-analyze -- make
