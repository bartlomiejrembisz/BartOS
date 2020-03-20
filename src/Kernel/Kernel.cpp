#include "Kernel/Kernel.h"
#include "Core/stdio.h"
#include "Core/stdlib.h"

#include "BitFields.h"

namespace ROS
{

Kernel::Kernel(const multiboot_info_t * const pMultibootInfo) :
    m_pMultibootInfo(pMultibootInfo),
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

extern "C" void kernel_main(const multiboot_info_t * const pMultibootInfo, unsigned int magic)
{
    if (MULTIBOOT_BOOTLOADER_MAGIC != magic)
    {
        // Print error to the screen and return.
        return;
    }

    using namespace ROS;

    Kernel kernel(pMultibootInfo);
    kernel.Run();
}
