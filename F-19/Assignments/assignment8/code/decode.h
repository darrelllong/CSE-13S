#ifndef __DECODE_H__
#define __DECODE_H__

//
// Handles LZW decompression algorithm.
// Decompresses contents of the input file to the output file.
//
// infile:     Input file to decompress.
// outfile:    Output file where decompressed data goes.
//
void decode(int infile, int outfile);

#endif
