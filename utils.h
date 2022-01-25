#ifndef __UTILS_H_
#define __UTILS_H_

uint64_t inv(uint64_t x, int len);
uint64_t rand64(void);
uint32_t mod_add(uint32_t a, uint32_t b);   // 模加 模数CONST_P为32位
uint32_t mod_sub(uint32_t a, uint32_t b);   // 模减 模数CONST_P为32位
uint32_t mod_inv(uint32_t x);               // 模逆
uint32_t mod_mul(uint32_t a, uint32_t b);   // 模乘

uint32_t generate_prime(uint32_t range);
int64_t mod_inv_fast(int64_t d, int64_t n); // 求模逆 采用扩展欧几里得算法

#endif