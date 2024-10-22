ENCODER  = encode
DECODER  = decode

ENCODERSRC = encode.c trie.c io.c util.c
DECODERSRC = decode.c word.c io.c util.c

ENCODEROBJ = $(ENCODERSRC:%.c=%.o)
DECODEROBJ = $(DECODERSRC:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror

all: $(ENCODER) $(DECODER)

$(ENCODER): $(ENCODEROBJ)
	$(CC) -o $@ $^

$(DECODER): $(DECODEROBJ)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(ENCODER) $(DECODER) *.o
