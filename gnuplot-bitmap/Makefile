CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -O2 -g -Ivendor/stb
LFLAGS = -lm
OBJS = gnuplot-bitmap.o stbi.o
RM = rm -f

all: gnuplot-bitmap

gnuplot-bitmap: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o gnuplot-bitmap

%.o: %.c
	$(CC) $(CFLAGS) -c $<

stbi.o: stbi.c vendor/stb/stb_image.h
	$(CC) $(CFLAGS) -c $< -Wno-unused-but-set-variable

clean:
	$(RM) gnuplot-bitmap $(OBJS)

scan-build: clean
	scan-build --use-cc=$(CC) make

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean scan-build format
