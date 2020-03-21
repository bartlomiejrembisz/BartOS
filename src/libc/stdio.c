#include "stdio.h"
#include "stdlib.h"
#include "io.h"

int putchar(int c)
{
    outb(0xe9, c);
    return c;
}

// ---------------------------------------------------------------------------------------------------------

int puts(const char *pStr)
{
    while (*pStr)
        putchar(*pStr++);

    return 1;
}
