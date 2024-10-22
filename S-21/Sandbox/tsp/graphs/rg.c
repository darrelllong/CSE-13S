#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char **argv) {
    int count = 10;
    srandom((long)time((void *)0));
    if (argc == 2) {
	count = atoi(argv[1]);
    }
    for (long i = 0; i < count; i += 1) {
	for (long j = 0; j < log(count) + random() % (long) log(count); j += 1) {
	    long v = random() % count;
	    if (j != v) {
	       printf("%ld %ld %ld\n", i, v, random() % 10);
	    }
	}
    }
    return 0;
}
