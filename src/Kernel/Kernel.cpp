#include "BartOS.h"

#include "Kernel/Arch/x86_64/CPU.h"
#include "Kernel/Memory/PhysicalMemoryManager.h"

extern "C" void kernel_main()
{
    using namespace BartOS;

    x86::CPU::Get().InitializeSse();
    x86::CPU::Get().InitializeGdt();

    if (!PhysicalMemoryManager::Get().Initialize())
    {
        kprintf("Memory map not found in multiboot_into.");
        return;
    }
}
