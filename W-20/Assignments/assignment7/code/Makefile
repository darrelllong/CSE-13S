ENCODER  = encode
DECODER  = decode

ENCODERSRC = encode.c trie.c io.c util.c
DECODERSRC = decode.c word.c io.c util.c

ENCODEROBJ = $(ENCODERSRC:%.c=%.o)
DECODEROBJ = $(DECODERSRC:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror

.PHONY: all clean infer

all: $(ENCODER) $(DECODER)

$(ENCODER): $(ENCODEROBJ)
	$(CC) -o $@ $^

$(DECODER): $(DECODEROBJ)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ENCODER) $(DECODER) *.o infer-out

infer: clean
	infer-capture -- make
	infer-analyze -- make
