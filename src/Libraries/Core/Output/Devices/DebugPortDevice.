#include "DebugPortDevice.h"

namespace BartOS
{

StatusCode DebugPortDevice::Write(const uint8_t * const pBuffer, size_t bufferLength)
{
    for (size_t i = 0; i < bufferLength; ++i)
        PrintChar(static_cast<char>(pBuffer[i]));

    return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

StatusCode DebugPortDevice::Write(const char *pStr)
{
    while (*pStr)
        PrintChar(*pStr++);

    return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

void DebugPortDevice::PrintChar(const char character)
{
    
    outb(IO_PORT_ADDR, character);
}

} // namespace BartOS
