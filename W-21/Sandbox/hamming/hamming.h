#ifndef HAM_H
#define HAM_H
#include <stdint.h>

typedef enum ham_rc { HAM_OK, HAM_ERR_OK, HAM_ERR = 0xFF } ham_rc;

uint8_t ham_init(void);

ham_rc ham_decode(uint8_t encoded, uint8_t *decoded);

uint8_t ham_encode(uint8_t val);

void ham_destroy(void);

#endif
