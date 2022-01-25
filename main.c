#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "des.h"

#define ITER 1000000

void test_encrypt_distrb(void) {
    uint64_t encrypt[ITER] = {0};
    uint64_t plain = 0, key = 0;
    uint64_t subKey[16] = {0};
    srand((unsigned int)time(NULL));

    for (int i = 0; i < ITER; i++) {
        plain = rand64() % ((uint64_t)CONST_P * (uint64_t)CONST_P);
        key = rand64() % ((uint64_t)CONST_P * (uint64_t)CONST_P);
        generate_subkeys(&key, subKey);
        encrypt[i] = modpDES_encrypt_decypt(plain, ENCRYPT, subKey);
    }

    FILE *fp = fopen("data_modpDES.csv", "w");
    for (int i = 0; i < ITER; i++) {
        fprintf(fp, "%lu,", encrypt[i]);
    }
    fclose(fp);

    return;
}

int main(void) {
    // plain_text encrypt_text decrypt_text key均应该小于p的平方
    uint64_t plain_text = 0, encrypt_text = 0, decrypt_text = 0, k = 0;
    uint64_t subKey[16] = {0};
    int start, end;
    int test_speed_iter = 10000000;

    srand((unsigned int)time(NULL));
    k = rand64() % ((uint64_t)CONST_P * (uint64_t)CONST_P);
    generate_subkeys(&k, subKey);
    plain_text = rand64() % ((uint64_t)CONST_P * (uint64_t)CONST_P);

    encrypt_text = modpDES_encrypt_decypt(plain_text, ENCRYPT, subKey);
    decrypt_text = modpDES_encrypt_decypt(encrypt_text, DECRYPT, subKey);

    // start = clock();
    // for (int i = 0; i < test_speed_iter; i++) {
    //     encrypt_text = DES_encrypt_decypt(plain_text, ENCRYPT, subKey);
    // }
    // end = clock();
    // printf("ENCRYPT: %fs for %d times\n", (double)(end - start) / CLOCKS_PER_SEC, test_speed_iter);
    // start = clock();
    // for (int i = 0; i < test_speed_iter; i++) {
    //     decrypt_text = DES_encrypt_decypt(encrypt_text, DECRYPT, subKey);
    // }
    // end = clock();
    // printf("DECRYPT: %fs for %d times\n", (double)(end - start) / CLOCKS_PER_SEC, test_speed_iter);

    // test_encrypt_distrb();

    return 0;
}