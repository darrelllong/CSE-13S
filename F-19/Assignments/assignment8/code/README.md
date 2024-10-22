## LZWCoder ##

This program is a C implementation of the LZW coding algorithm. LZW coding is a
lossless data compression algorithm that makes use of summarizing long-repeated
patterns in text into a code.

### Design ###

This implementation uses variable code widths, where the length of each buffered
code in bits is determined by the next available code. Codes are 16-bit unsigned
integers. This implementation also flushes the dictionary created during
compression and decompression when the next available code hits UINT16_MAX, a
macro defined in <inttypes.h> as the maximum value of a unsigned 16-bit integer.
Standard input and output are used by default if an input or output isn't
specified, respectively.

### Program Usage ###

To build the binary:

    make

To compress a file:

    $ ./lzwcoder -c -i <file to compress> -o <file to compress to>
    $ ./encode -i <file to compress> -o <file to compress to>

To decompress a file:

    $ ./lzwcoder -d -i <file to decompress> -o <file to decompress to>
    $ ./decode -i <file to decompress> -o <file to decompress to>

To enable verbose flag to show statistics:

    $ ./lzwcoder -v [... previously mentioned flags ...]

To remove all compile-time generated files:

    make spotless

To remove everything including the binary:

    make clean
