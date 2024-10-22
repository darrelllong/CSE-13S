#ifndef __HAM_H__
#define __HAM_H__
#include <stdint.h>

typedef enum ham_rc { HAM_ERR = -1, HAM_OK = 0, HAM_ERR_OK = 1 } ham_rc;

ham_rc ham_init(void);

void ham_destroy(void);

ham_rc ham_encode(uint8_t data, uint8_t *code);

ham_rc ham_decode(uint8_t code, uint8_t *data);

#endif
