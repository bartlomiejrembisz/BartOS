#include "BartOS.h"

#include "Arch/i386/CPU.h"

extern "C" void kernel_main(const multiboot_info_t * const pMultibootInfo, unsigned int magic)
{
    using namespace BartOS;
    if (MULTIBOOT_BOOTLOADER_MAGIC != magic)
    {
        // Print error to the screen and return.
        x86::CPU::Abort();
    }

    // Store the multiboot header in a remote place.
    x86::CPU::m_pMultibootInfo = pMultibootInfo;

    x86::CPU::GetInstance().InitializeGdt();
}
