#ifndef DEBUG_PORT_DEVICE_H
#define DEBUG_PORT_DEVICE_H

#include "IPrintableDevice.h"

namespace ROS
{

/*
 *  @brief The DebugPortDevice class.
 *  
 *  Used to output strings to the debug port.
 */
class DebugPortDevice : public IPrintableDevice
{
public:
    static const uint16_t IO_PORT_ADDR = 0xe9;

    //! IPrintableDevice interface.
    virtual StatusCode Write(const uint8_t * const pBuffer, size_t bufferLength) override;
    virtual StatusCode Write(const char *pStr) override;

private:
    //! Print a character
    void PrintChar(const char character);
};

} // namespace ROS

#endif // DEBUG_PORT_DEVICE_H