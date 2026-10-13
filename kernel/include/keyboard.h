#pragma once
#include "types.h"
#include "io.h"
#include "terminal.h"

extern char keybordBuffer[256];

void gets(char* buf, int count, int pos);