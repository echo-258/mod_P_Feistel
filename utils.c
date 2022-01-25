#include <stdint.h>
#include <stdlib.h>
#include "des.h"
#include "utils.h"

// 将长度为len比特的数字所有比特位进行逆序排列
uint64_t inv(uint64_t x, int len) {
    uint64_t ret = 0;
    int x_pos = 0, ret_pos = 0;
    uint64_t bit = 0;
    for (int i = 0; i < len; i++) {
        x_pos = len - i - 1;
        bit = (uint64_t)1 << x_pos;
        bit &= x;
        bit >>= x_pos;
        bit <<= i;
        ret |= bit;
    }
    return ret;
}

// 产生一个64位随机数
uint64_t rand64(void) {
    uint64_t res;
    int cnt = 4;
    while (cnt--) {
        res <<= 16;
        res |= rand() & 0xffff;
    }
    return res;
}

// 模加 模数CONST_P为32位
uint32_t mod_add(uint32_t a, uint32_t b) {
    uint64_t temp = (uint64_t)a + (uint64_t)b;
    uint64_t ret = temp % CONST_P;
    return (uint32_t)ret;
}

// 模减 模数CONST_P为32位
uint32_t mod_sub(uint32_t a, uint32_t b) {
    uint64_t temp = CONST_P + (uint64_t)a - (uint64_t)b;
    uint64_t ret = temp % CONST_P;
    return (uint32_t)ret;
}

// 模逆
uint32_t mod_inv(uint32_t x) {
    if (x == 0) {
        return 0;
    }
    uint64_t ret = 0;
    for (ret = 1; ret < CONST_P; ret++) {
        if (ret * (uint64_t)x % CONST_P == 1) {
            break;
        }
    }
    return (uint32_t)ret;
}

// 模乘
uint32_t mod_mul(uint32_t a, uint32_t b) {
    uint64_t temp = (uint64_t)a * (uint64_t)b;
    uint64_t ret = temp % 32;
    return (uint32_t)ret;
}

uint64_t _mod_pow(uint64_t a, uint64_t n, uint64_t mod) {
    uint64_t res = 1;
    while (n) {
        if (n & 1)
            res = res * a % mod;
        a = a * a % mod;
        n >>= 1;
    }
    return res;
}

// Miller-Rabin随机算法检测n是否为素数
int Miller_Rabin(uint64_t n) {
    if (n == 2)
        return 1;
    if (n < 2 || !(n & 1))
        return 0;
    uint64_t m = n - 1, k = 0;
    while (!(m & 1)) {
        k++;
        m >>= 1;
    }
    for (int i = 1; i <= 25; i++) // 迭代测试
    {
        uint64_t a = rand() % (n - 1) + 1;
        uint64_t x = _mod_pow(a, m, n);
        uint64_t y;
        for (int j = 1; j <= k; j++) {
            y = x * x % n;
            if (y == 1 && x != 1 && x != n - 1)
                return 0;
            x = y;
        }
        if (y != 1)
            return 0;
    }
    return 1;
}

uint32_t generate_prime(uint32_t range) {
    uint32_t ret = 0;
    int not_found = 1;
    while (not_found) {
        // ret = rand();
        ret = rand() % range;
        if (ret < 6 || ret % 2 == 0) {
            continue;
        }
        if (Miller_Rabin((uint64_t)ret) == 1) {
            not_found = 0;
        }
    }
    return ret;
}

// 求模逆 采用扩展欧几里得算法
int64_t mod_inv_fast(int64_t d, int64_t n) {
    int64_t a, b, q, r;     // a为被除数，b为除数，q为商
    int64_t u = 0, v = 1, t;
    int64_t ret;
    a = n;
    b = d % n;
    while (b != 0) {
        q = (int64_t)a / b;
        r = a - b * q;
        a = b;
        b = r;
        t = v;
        v = u - q * v;
        u = t; //把每次没有进行运算的v的值给u
    }          //辗转相除，直到余数b=0，跳出循环
    if (a != 1)
        return 0;
    ret = ((u < 0) ? u + n : u); //返回的值都是mod(n)后的值，使得最后的值是正数；
    return ret;
}