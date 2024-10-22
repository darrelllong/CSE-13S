#include "bf.h"

#include "bv.h"
#include "salts.h"
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
        // Grimm's Fairy Tales
        b->primary[0] = SALT_PRIMARY_LO;
        b->primary[1] = SALT_PRIMARY_HI;
        // The Adventures of Sherlock Holmes
        b->secondary[0] = SALT_SECONDARY_LO;
        b->secondary[1] = SALT_SECONDARY_HI;
        // The Strange Case of Dr. Jekyll and Mr. Hyde
        b->tertiary[0] = SALT_TERTIARY_LO;
        b->tertiary[1] = SALT_TERTIARY_HI;
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

uint32_t bf_size(BloomFilter *b) {
    return bv_length(b->filter);
}

void bf_insert(BloomFilter *b, char *key) {
    bv_set_bit(b->filter, hash(b->primary, key) % bv_length(b->filter));
    bv_set_bit(b->filter, hash(b->secondary, key) % bv_length(b->filter));
    bv_set_bit(b->filter, hash(b->tertiary, key) % bv_length(b->filter));
    return;
}

bool bf_probe(BloomFilter *b, char *key) {
    return bv_get_bit(b->filter, hash(b->primary, key) % bv_length(b->filter))
           && bv_get_bit(b->filter, hash(b->secondary, key) % bv_length(b->filter))
           && bv_get_bit(b->filter, hash(b->tertiary, key) % bv_length(b->filter));
}

uint32_t bf_count(BloomFilter *b) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bv_length(b->filter); i += 1) {
        count += bv_get_bit(b->filter, i);
    }
    return count;
}
