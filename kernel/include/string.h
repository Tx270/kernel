#pragma once
#include "types.h"

void* memcpy(void* dst, const void* src, uint32_t n);
int strlen(const char* s);
int strcmp(char* s01, char* s02);
bool streq(char* s0, char* s1);

#define memset_t(dst, type, val, count) \
    do { \
        type *_dst = (type *)(dst); \
        type _tmp = (val); \
        for (int _i = 0; _i < (count); _i++) { \
            _dst[_i] = _tmp; \
        } \
    } while (0)
