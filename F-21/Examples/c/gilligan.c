#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SPREAD (1<<20)

int main(void) {
    uint32_t coconuts = 0;
    double inside = 0.0, total = 0.0;
    printf("How many coconuts? ");
    scanf("%" PRIu32, &coconuts);
    srandom(time(NULL));
    for (uint32_t i = 0; i < coconuts; i += 1) {
        double x = random() % SPREAD / (double) (SPREAD - 1);
        double y = random() % SPREAD / (double) (SPREAD - 1);
        if (sqrt(x * x + y * y) <= 1.0) {
            inside += 1.0;
        }
        total += 1.0;
    }
    printf("%lf\n", 4.0 * inside / total);
    return 0;
}
