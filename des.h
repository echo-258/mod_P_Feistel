#ifndef __DES_H_
#define __DES_H_

#define ENCRYPT 0
#define DECRYPT 1
#define BLOCKSIZE 64
#define CONST_P 65537
#define CONST_COEF 5

#define ROUND_RIGHT_SHIFT(X, SHM, LEN) X = (X << (LEN - SHM)) | (X >> SHM)

typedef struct lr {
    uint32_t l;
    uint32_t r;
} LR;

// uint32_t CONST_P = 7;

uint64_t modpDES_encrypt_decypt(uint64_t data, uint8_t mode, uint64_t *subKey);
void generate_subkeys(uint64_t *_key, uint64_t *subKey);

#endif
