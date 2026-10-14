#pragma once
#include "types.h"

static inline uint8_t inw(uint16_t port)
{
    uint16_t ret;

    __asm__ volatile (
        "inw %w1, %0"
        : "=a"(ret)
        : "d"(port)
    );

    return ret;
}

static inline void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile (
        "outw %0, %w1"
        :
        : "a"(value), "d"(port)
    );
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;

    __asm__ volatile (
        "inb %w1, %0"
        : "=a"(ret)
        : "d"(port)
    );

    return ret;
}

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile (
        "outb %0, %w1"
        :
        : "a"(value), "d"(port)
    );
}