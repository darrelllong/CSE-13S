COM_OBJ = bv.o hamming.o
GEN_OBJ = generator.o $(COM_OBJ)
DEC_OBJ = decoder.o $(COM_OBJ)
ERR_OBJ = error.o

EXE = err gen dec

CC = clang
CFLAGS = -Wextra -Werror -pedantic -std=c17 

.PHONY: all clean format

all: gen dec err

gen: $(GEN_OBJ)
	$(CC) -o $@ $(CFLAGS) $(GEN_OBJ)

dec: $(DEC_OBJ)
	$(CC) -o $@ $(CFLAGS) $(DEC_OBJ)

err: $(ERR_OBJ)
	$(CC) -o $@ $(CFLAGS) $(ERR_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -rf $(GEN_OBJ) $(DEC_OBJ) $(ERR_OBJ) $(EXE)
	
format:
	@clang-format -i *.[ch]
