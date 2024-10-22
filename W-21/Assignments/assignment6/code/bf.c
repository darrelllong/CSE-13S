#include "bf.h"
#include "bv.h"
#include "speck.h"
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *b = (BloomFilter *)malloc(sizeof(BloomFilter));
    if (b) {
        b->primary[0]   = 0xFC28CA6885711CF7;
        b->primary[1]   = 0x2841AF568222F773;
        b->secondary[0] = 0x85AE998311115AE3;
        b->secondary[1] = 0xB6FAC2AE33A40089;
        b->tertiary[0]  = 0xD37B01DF0AE8F8D0;
        b->tertiary[1]  = 0x911D454886CA7CF7;
        b->filter = bv_create(size);
        if (!b->filter) {
            free(b);
            b = NULL;
        }
    }
    return b;
}

void bf_delete(BloomFilter **b) {
    bv_delete(&(*b)->filter);
    free(*b);
    *b = NULL;
}

uint32_t bf_length(BloomFilter *b) {
    return bv_length(b->filter);
}

void bf_insert(BloomFilter *b, char *key) {
    bv_set_bit(b->filter, hash(b->primary, key) % bv_length(b->filter));
    bv_set_bit(b->filter, hash(b->secondary, key) % bv_length(b->filter));
    bv_set_bit(b->filter, hash(b->tertiary, key) % bv_length(b->filter));
    return;
}

bool bf_probe(BloomFilter *b, char *key) {
    return bv_get_bit(b->filter, hash(b->primary, key) % bv_length(b->filter)) &&
        bv_get_bit(b->filter, hash(b->secondary, key) % bv_length(b->filter)) &&
        bv_get_bit(b->filter, hash(b->tertiary, key) % bv_length(b->filter));
}

uint32_t bf_count(BloomFilter *b) {
    uint32_t count = 0;

    for (uint32_t i = 0; i < bv_length(b->filter); i += 1) {
        count += bv_get_bit(b->filter, i);
    }

    return count;
}
