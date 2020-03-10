#include <Core/stdio.h>
#include <Core/stdlib.h>

#include <Kernel/Uart.h>

#ifdef __cplusplus
extern "C" {
#endif

char getc(void)
{
    return ROS::Uart::GetInstance().GetByte();
    
}

// ---------------------------------------------------------------------------------------------------------

void putc(char c)
{
    ROS::Uart::GetInstance().PutByte(static_cast<uint8_t>(c));
}

// ---------------------------------------------------------------------------------------------------------

void puts(const char *pStr)
{
    int i;
    for (i = 0; pStr[i] != '\0'; i ++)
        putc(pStr[i]);
}

// ---------------------------------------------------------------------------------------------------------

void gets(char *pBuffer, int bufferLength)
{
    int i;
    char c;

    for (i = 0; (c = getc()) != '\r' && bufferLength > 1; i++, bufferLength--)
    {
        putc(c);
        pBuffer[i] = c;
    }

    putc('\n');
    if (c == '\n') {
        pBuffer[i] = '\0';
    }
    else
        pBuffer[bufferLength - 1] = '\0';
}

#ifdef __cplusplus
}
#endif
