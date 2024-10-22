#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAGIC     0xdeadcafe
#define ALPHABET  256
#define BLOCK     4096
#define HALFWORD  16
#define BYTE      8

#ifdef DEBUG
#define DEBUG_MODE 1
#else
#define DEBUG_MODE 0
#endif

//
// Asserts validity of expression and prints out error if needed.
// Prints out file, line, and function if debug mode is enabled.
//
// expr:  Expression to validate.
// fmt:   Format string for error message to print if needed.
// ...:   Variable args for format string.
//
#define ASSERT(expr, fmt, ...)                                                 \
  do {                                                                         \
    if (!(expr)) {                                                             \
      if (DEBUG_MODE) {                                                        \
        fprintf(stderr, "%s:%d:%s(): ", __FILE__, __LINE__, __func__);         \
      }                                                                        \
      fprintf(stderr, fmt, __VA_ARGS__);                                       \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

// Global variable to track exit status.
extern int exit_status;

//
// Reads bytes into a specified buffer from the input file.
// A wrapper for read() since pipes don't always pass all the bytes.
//
// infile:  The input file to read from.
// buffer:  The buffer to read bytes into.
// to_read: The number of bytes to read.
//
void read_buffer(int infile, uint8_t *buffer, int to_read);

//
// Writes bytes from a specified buffer to the output file.
// A wrapper for write() since pipes don't always pass all the bytes.
//
// outfile:   The output file to write to.
// buffer:    The buffer to write bytes from.
// to_write:  The number of bytes to write.
//
void write_buffer(int outfile, uint8_t *buffer, int to_write);

//
// Prints out custom error message and changes exit status.
//
// format:     The string to print when reporting an error.
// ...:        Variable arguments for the string's format.
//
void errprint(const char *format, ...);

//
// Checks if file descriptor is a pipe.
//
// fd:  The file descriptor.
//
bool ispipe(int fd);

//
// Returns file descriptor for a temporary stdin file.
// Used for reading input from stdin if stdin is a terminal.
//
// pipe:  True if stdin is a pipe else false.
//
int mkstemp_stdin(bool pipe);

//
// Calculates the log base 2 of a 64-bit integer.
//
// n:  The integer to find the log base 2 of.
//
uint8_t log2_64(uint64_t n);

//
// Max function to return max of two 64-bit integers.
//
// x:   The first integer.
// y:   The second integer.
//
uint64_t max(uint64_t x, uint64_t y);

//
// Converts bit length into minimum number of bytes.
//
// bits:   The length in bits.
//
uint64_t bytes(uint64_t bits);

#endif
