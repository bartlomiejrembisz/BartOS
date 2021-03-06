#include "stdio.h"
#include "stdlib.h"
#include "Kernel/Arch/x86_64/io.h"

int putchar(int c)
{
    out_byte(0xe9, c);
    return c;
}

// ---------------------------------------------------------------------------------------------------------

int puts(const char *pStr)
{
    while (*pStr)
        putchar(*pStr++);

    return 1;
}
