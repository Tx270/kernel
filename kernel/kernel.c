#include "include/shell.h"
#include "include/terminal.h"
#include "include/keyboard.h"


void kernel_main(void)
{
    shell_command("clear");

    for (;;) {
        puts(" > ", 0x04, terminalCol * 80);
        gets(keybordBuffer, 256, (terminalCol * 80) + 3);
        shell_command(keybordBuffer);
    }
}