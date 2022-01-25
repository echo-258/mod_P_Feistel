#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "tables.h"
#include "des.h"
#include "utils.h"

// 选择置换函数
void transform(uint64_t* dst, uint64_t* src, uint8_t* shmt, uint32_t length) {
    uint64_t bit_pos;
    uint64_t tmp;
    uint32_t shift = 0;
    *dst = 0;
    for (int i = 0; i < length; i++) {
        shift = shmt[i] - 1;
        bit_pos = 1;
        bit_pos <<= shift; // 根据选择表，找到要从src中取比特位的位置
        *dst |= (bit_pos & *src) >> shift << i;
    }
}

void generate_subkeys(uint64_t* _key, uint64_t* subKey) {
    // 密钥置换1
    uint64_t* key = (uint64_t*)malloc(sizeof(uint64_t));
    transform(key, _key, replacement[0], 56);
    uint64_t C, D, buffer;
    C = D = 0;
    C = *key & 0xfffffff;
    D = (*key & 0xfffffff0000000) >> 28;

    // 循环16轮，生成 k1 - k16
    for (int i = 0; i < 16; i++) {
        // 循环左移
        ROUND_RIGHT_SHIFT(C, key_shift[i], 28);
        ROUND_RIGHT_SHIFT(D, key_shift[i], 28);
        C &= 0xfffffff;
        D &= 0xfffffff;
        buffer = ((D << 28) | C);
        // 密钥置换2
        transform((subKey + i), &buffer, replacement[1], 56);
    }
    free(key);

    for (int i = 0; i < 16; i++) {
        subKey[i] %= CONST_P;     // 现在 所有轮密钥在0到p-1之间
    }
    return;
}

// uint32_t round_func(uint32_t R, uint64_t k) {
//     uint64_t ret = mod_add(R, (uint32_t)k);
//     return ret;
// }

uint32_t round_func(uint32_t R, uint64_t k) {
    uint32_t ret = 0;
    // ret = mod_inv(R);
    ret = mod_inv_fast(R, CONST_P);
    ret = mod_mul(ret, CONST_COEF);
    ret = mod_add(ret, (uint32_t)k);
    return ret;
}

// 对一个分组进行加密或解密 mode=0加密 mode=1解密
uint64_t modpDES_encrypt_decypt(uint64_t data, uint8_t mode, uint64_t* subKey) {
    // uint64_t group = 0;     // 加密/解密过程中的一个分组
    uint64_t ret = 0;       // 加密/解密后的结果
    uint32_t half = 0;

    // 初始置换
    // transform(&group, &data, IPTable[0], 64);
    // uint64_t group_FR = inv(group, 64);

    LR lr;
    // lr.l = group & 0xffffffff;
    // lr.r = (group & 0xffffffff00000000) >> 32;
    lr.l = data / CONST_P;
    lr.r = data % CONST_P;

    // 逐轮操作
    for (int j = 0; j < 16; j++) {
        if (mode == ENCRYPT) {
            // lr = round_operate(lr.l, lr.r, subKey[j]);
            half = lr.r;
            lr.r = mod_add(lr.l, round_func(lr.r, subKey[j]));
            // lr.r = mod_sub(lr.l, round_func(lr.r, subKey[15 - j]));
            lr.l = half;
        } else if (mode == DECRYPT) {
            // lr = round_operate(lr.l, lr.r, subKey[15 - j]);
            half = lr.r;
            lr.r = mod_sub(lr.l, round_func(lr.r, subKey[15 - j]));
            // lr.r = mod_add(lr.l, round_func(lr.r, subKey[j]));
            lr.l = half;
        }
    }
    // group = ((uint64_t)lr.l << 32) | lr.r;
    // ret = (uint64_t)lr.l * CONST_P + (uint64_t)lr.r;
    ret = (uint64_t)lr.r * CONST_P + (uint64_t)lr.l;

    // 逆初始置换
    // transform(&ret, &group, IPTable[1], 64);
    return ret;
}
