# Simon and Simple Simon 128/256 in C

This project is comprised of two programs: the encryptor, ```encrypt```, and the
decryptor, ```decrypt```.

Together, they perform the functionality of Simon block cipher, presented by the
NSA.

Both can also perform Simple Simon 128/256 through a command-line option.

### Design

The code in this project was written following the pseudocode supplied by the
NSA.

Both the encryptor and decryptor operate on inputs of two 64-bit words, or a
block size of 128 bits.

Both the encryptor and decryptor must be supplied a symmetric 256-bit key.

### Building

To build the encryptor and decryptor:

	$ make

Or:

	$ make all

### Usage

To encrypt an input block with a 256-bit key (```stdin``` and ```stdout``` are
the default for input and output, respectively):

	$ ./encrypt -k key [-i input] [-o output]

To decrypt an input block with a 256-bit key (```stdin``` and ```stdout``` are
the default for input and output, respectively):

	$ ./decrypt -k key [-i input] [-o output]

### Additional Program Options

Both the encryptor and decryptor support the ```getopt()``` command-line options
below.

	'-h': Display program help and usage
	'-v': Display verbose program output
	'-s': Enable Simple Simon 128/256
