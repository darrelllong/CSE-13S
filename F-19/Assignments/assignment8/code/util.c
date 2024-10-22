#include "util.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

// Global variable to track exit status.
int exit_status = EXIT_SUCCESS;

//
// Reads bytes into a specified buffer from the input file.
// A wrapper for read() since pipes don't always pass all the bytes.
//
// infile:  The input file to read from.
// buffer:  The buffer to read bytes into.
// to_read: The number of bytes to read.
//
void read_buffer(int infile, uint8_t *buffer, int to_read) {
  int bytes_to_read = to_read;
  int bytes_read = 0;
  int total_read = 0;

  do {
    bytes_read = read(infile, buffer + total_read, bytes_to_read);
    ASSERT(bytes_read != -1, "%s\n", "Failed to read bytes from infile.");
    bytes_to_read -= bytes_read;
    total_read += bytes_read;
  } while (bytes_read > 0 && total_read != to_read);

  return;
}

//
// Writes bytes from a specified buffer to the output file.
// A wrapper for write() since pipes don't always pass all the bytes.
//
// outfile:   The output file to write to.
// buffer:    The buffer to write bytes from.
// to_write:  The number of bytes to write.
//
void write_buffer(int outfile, uint8_t *buffer, int to_write) {
  int bytes_to_write = to_write;
  int bytes_written = 0;
  int total_written = 0;

  do {
    bytes_written = write(outfile, buffer + total_written, bytes_to_write);
    ASSERT(bytes_written != -1, "%s\n", "Failed to write bytes to outfile.");
    bytes_to_write -= bytes_written;
    total_written += bytes_written;
  } while (bytes_written > 0 && total_written != to_write);

  return;
}

//
// Modification of printf.
// Prints out custom error message and changes exit status.
//
// format:     The string to print.
// ...:        Variable arguments for the string's format.
//
void errprint(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  vfprintf(stdout, format, arg);
  va_end(arg);
  exit_status = EXIT_FAILURE;
}

//
// Checks if file descriptor is a pipe.
//
// fd:  The file descriptor.
//
bool ispipe(int fd) {
  struct stat statbuf;
  fstat(fd, &statbuf);
  return S_ISFIFO(statbuf.st_mode) ? true : false;
}

//
// Returns file descriptor for a temporary stdin file.
// Used for reading input from stdin if stdin is a terminal.
//
// pipe:  True if stdin is a pipe else false.
//
int mkstemp_stdin(bool pipe) {
  char temp[] = "/tmp/lzwcoderXXXXXX";
  int tempfile = mkstemp(temp);

  uint8_t buffer[BLOCK] = { 0 };
  int bytes = 0;
  int fd = pipe ? STDIN_FILENO : STDOUT_FILENO;

  while ((bytes = read(fd, buffer, BLOCK)) > 0) {
    write_buffer(tempfile, buffer, bytes);
  }

  lseek(tempfile, 0, SEEK_SET);
  unlink(temp);

  return tempfile;
}

//
// Calculates the log base 2 of a 64-bit integer.
//
// n:  The integer to find the log base 2 of.
//
uint8_t log2_64(uint64_t n) {
  uint8_t ret = 0;

  while (n > 1) {
    ret += 1;
    n >>= 1;
  }

  return ret;
}

//
// Max function to return max of two 64-bit integers.
//
// x:   The first integer.
// y:   The second integer.
//
uint64_t max(uint64_t x, uint64_t y) {
  return (x > y) ? x : y;
}

//
// Converts bit length into minimum number of bytes.
//
// bits:   The length in bits.
//
uint64_t bytes(uint64_t bits) {
  return ((bits % BYTE) || (bits < BYTE)) ? bits / BYTE + 1 : bits / BYTE;
}
