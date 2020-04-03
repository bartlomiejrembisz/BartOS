#ifndef IPRINTABLE_DEVICE_H
#define IPRINTABLE_DEVICE_H

#include "Kernel/BartOS.h"

namespace BartOS
{

/*
 *  @brief The IPrintableDevice interface.
 *  Used by the output stream class.
 */
class IPrintableDevice
{
public:
    /*
     *  @brief Write buffer to device.
     * 
     *  @param pBuffer pointer to buffer.
     *  @param bufferLength the length of buffer.
     */
    virtual StatusCode Write(const uint8_t * const pBuffer, size_t bufferLength) = 0;

    /*
     *  @brief Write string to device.
     * 
     *  @param pStr pointer to the string.
     */
    virtual StatusCode Write(const char *pStr) = 0;
};

} // namespace BartOS

#endif // IPRINTABLE_DEVICE_H