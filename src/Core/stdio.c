#include <Core/stdio.h>
#include <Core/stdlib.h>
#include "Misc/io.h"

#ifdef __cplusplus
extern "C" {
#endif

char getc(void)
{
}

// ---------------------------------------------------------------------------------------------------------

void putc(char c)
{
    outb(0xe9, c);
}

// ---------------------------------------------------------------------------------------------------------

void puts(const char *pStr)
{
    while (*pStr)
        putc(*pStr++);
}

// ---------------------------------------------------------------------------------------------------------

void gets(char * const pBuffer, int bufferLength)
{
    int i;
    char c;

    for (i = 0; (c = getc()) != '\r' && bufferLength > 1; i++, bufferLength--)
    {
        if (0x08 != c)
            pBuffer[i] = c;
        else
            i = (0 == (i - 1)) ? 0 : i - 2;
    }

    if (c == '\n') 
        pBuffer[i] = '\0';
    else
        pBuffer[bufferLength - 1] = '\0';
}

#ifdef __cplusplus
}
#endif
