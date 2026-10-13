#include "../include/string.h"

void* memcpy(void* dst, const void* src, uint32_t n) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src; 

    for (uint8_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    
    return dst;
}