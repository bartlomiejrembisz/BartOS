#include <Kernel/Arch/x86_64/Interrupts/InterruptDescriptorTable.h>
#include "BartOS.h"

#include "Kernel/Arch/x86_64/GDT.h"
#include "Kernel/Arch/x86_64/CPU.h"
#include "Kernel/Memory/PhysicalMemoryManager.h"

extern "C" void kernel_main()
{
    using namespace BartOS;

    x86_64::GDT::Get().Initialize();
    x86_64::IDT::IDT::Get().Initialize();

    if (!PhysicalMemoryManager::Get().Initialize())
    {
        return;
    }
}
