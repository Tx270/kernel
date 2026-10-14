#include "../include/shell.h"
#include "../include/string.h"
#include "../include/io.h"
#include "../include/terminal.h"
#include "../include/colors.h"
#include "../include/rtc.h"


static void quit(void)
{
    outw(0x604, 0x2000);
}

static void greet(void)
{
    puts("GizzOS 0.0.1", COLOR_ACCENT, terminalCol * 80);
}

static void clear(void)
{
    memset_t(vga, uint16_t, 0x0F20, 80 * 25);
    terminalCol = 0;
    greet();
}

static void help(void)
{
    puts("Available commands:", COLOR_INFO, terminalCol++ * 80);
    puts("  die   - Shut down the system", COLOR_DEFAULT, terminalCol++ * 80);
    puts("  clear - Clear the screen", COLOR_DEFAULT, terminalCol++ * 80);
    puts("  greet - Display the system name", COLOR_DEFAULT, terminalCol++ * 80);
    puts("  help  - List available commands", COLOR_DEFAULT, terminalCol++ * 80);
    puts("  time  - Get current time according to mr Sebastian", COLOR_DEFAULT, terminalCol++ * 80);
}

static void time(void)
{
    RTCDateTime time;
    rtc_read(&time);

    char buffer[3];

    buffer[0] = '0' + time.hour / 10;
    buffer[1] = '0' + time.hour % 10;
    buffer[2] = '\0';

    puts("Hour: ", 0x0F, terminalCol * 80);
    puts(buffer, 0x0F, terminalCol * 80 + 6);
}

typedef void (*command_fn)(void);

typedef struct {
    const char *name;
    command_fn execute;
} ShellCommand;

static const ShellCommand commands[] = {
    { "die",  quit  },
    { "clear", clear },
    { "greet", greet },
    { "help",  help  },
    { "time",  time  },
};

void shell_command(char command[])
{
    if (streq(command, ""))
        return;

    for (unsigned int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (streq(command, (char *)commands[i].name)) {
            commands[i].execute();
            terminalCol++;
            return;
        }
    }

    puts("Unknown command", COLOR_ERROR, terminalCol * 80);
    terminalCol++;
}