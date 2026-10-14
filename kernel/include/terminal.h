#pragma once
#include "types.h"

extern volatile uint16_t *vga;
extern volatile uint8_t  terminalCol;

void putc(char character, uint16_t color, int pos);
void puts(char *string, uint16_t color, int pos);