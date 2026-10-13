#include "include/string.h"
#include "include/terminal.h"
#include "include/keyboard.h"


void kernel_main(void)
{
    memset_t(vga, uint16_t, 0x0F20, 80 * 25);
    puts("Konichiwa Grzegorz", 0x03, 0);

    int col = 1;

    for (;;) {
        // while (!(inb(0x64) & 0x01));
        puts(" > ", 0x04, col * 80);
        gets(keybordBuffer, 256, (col * 80) + 3);
        col++;
    }
}