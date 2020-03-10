#include <cstddef>
#include <stdint.h>

#include "Kernel/Kernel.h"
#include "Kernel/Uart.h"
#include "Core/stdio.h"

namespace ROS
{

Kernel::Kernel() :
    m_isShuttingDown(false)
{
}

// ---------------------------------------------------------------------------------------------------------

Kernel::~Kernel()
{
}

// ---------------------------------------------------------------------------------------------------------

void Kernel::Run()
{
    while (!m_isShuttingDown)
    {
        putc(getc());
        putc('\n');
    }
}

} // namespace ROS

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    ROS::Kernel kernel;
    kernel.Run();
}