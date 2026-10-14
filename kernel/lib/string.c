#include "../include/string.h"

void* memcpy(void* dst, const void* src, uint32_t n) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src; 

    for (uint8_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    
    return dst;
}

int strlen(const char* s) {
    int n = 0;
    while (s[n]) {
        n++;
    }
    return n;
}

int strcmp(char* s0, char* s1) {
    while(*s0 && (*s0 == *s1)) {
        s0++;
        s1++;
    }
    return *(const uint8_t*)s0 - *(const uint8_t*)s1;
}

bool streq(char* s0, char* s1) {
    return strcmp(s0, s1) == 0;
}