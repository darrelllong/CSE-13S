#include "bubble.h"
#include "util.h"
#include <inttypes.h>
#include <stdbool.h>

void bubble_sort(uint32_t *arr, uint32_t length) {
    bool swapped = true;
    
    while (swapped) {
        swapped = false;
        
        for (uint32_t i = 1; i < length; i += 1) {
            if (compare(arr[i], arr[i - 1])) {
                swap(&arr[i - 1], &arr[i]);
                swapped = true;
            }
        }
        
        length -= 1;
    }
    
    return;
}
