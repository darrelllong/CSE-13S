#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include "bv.h"
#include "sieve.h"

#define DEFAULT_LIMIT 100000
#define OPTIONS "n: "

//Get the next prime in the bitvector
uint32_t next_prime(BitVector *v, uint32_t p) {
  do{
    p++;
  } while(bv_get_bit(v, p) != 1);
  return p;
}

void print_factorization(BitVector *v){
  //We know that 2 is prime.
  printf("2 P\n");

  for (uint32_t i = 3; i <= bv_get_len(v); i++) {
    printf("%d ", i);

    if (bv_get_bit(v, i) == 1) {
      printf("P");
    } else {
      printf("C: ");

      uint32_t j = 2;
      uint32_t num = i;
      while (j <= num) {
        if (num % j == 0) {
	  printf("%d ", j);
	  num = num/j;
	  if (num == 1) {
            break;
	  }
	} else {
          j = next_prime(v, j);
        }
      }
    }
    printf("\n");
  }
  return;
}

int main(int argc, char *argv[]){
  BitVector *V = NULL;
  int c = 0;
  int n = DEFAULT_LIMIT;

  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
      case 'n':
        n = atoi(optarg);
	break;
      default:
	if (argc != 1) { printf("Invalid argument\n"); }
	break;
    }
  }

  V = bv_create(n);
  sieve(V);
  print_factorization(V);
  bv_delete(V);
  return 0;
}
