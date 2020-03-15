#include "Kernel/Kernel.h"
#include "Core/stdio.h"
#include "Core/stdlib.h"

#include <stddef.h>
#include <stdint.h>

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
    }
}

} // namespace ROS

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

extern "C" void kernel_main()
{

    ROS::Kernel kernel;
    kernel.Run();
}