#include "../include/terminal.h"

volatile uint16_t *vga = (volatile uint16_t *)0xB8000;
volatile uint8_t terminalCol = 1;

void putc(char character, uint16_t color, int pos)
{
    vga[pos] = ((uint16_t)color << 8) | (uint16_t)character;
}

void puts(char *string, uint16_t color, int pos)
{
    int n = 0;
    while (string[n]) {
        putc(string[n], color, pos+n);
        n++;
    }
}