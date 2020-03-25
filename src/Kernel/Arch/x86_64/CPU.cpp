#include "CPU.h"
#include "GDT.h"

#include "Libraries/libc/stdio.h"

#include "load_gdt.h"

namespace BartOS
{

namespace x86
{

const multiboot_info_t * CPU::m_pMultibootInfo = nullptr;

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

CPU::CPU()
{
}

// ---------------------------------------------------------------------------------------------------------

CPU::~CPU()
{
}

// ---------------------------------------------------------------------------------------------------------

void CPU::InitializeGdt()
{
    const uint32_t kernelCodeOffset = GDT::GetSelectorOffset(GDT::KERNEL_CODE);
    const uint32_t kernelDataOffset = GDT::GetSelectorOffset(GDT::KERNEL_DATA);

    m_globalDescriptorTable.Initialize();

    //load_gdt(&m_globalDescriptorTable, kernelCodeOffset, kernelDataOffset);

    kprintf("GDT initialized\n");
}

// ---------------------------------------------------------------------------------------------------------

void CPU::InitializeIdt()
{
    kprintf("IDT initialized\n");
}

// ---------------------------------------------------------------------------------------------------------

void CPU::InitializeSse()
{
}

// ---------------------------------------------------------------------------------------------------------

void CPU::Abort()
{
    
}

// ---------------------------------------------------------------------------------------------------------

void CPU::Cli()
{
    asm volatile("cli");
}

// ---------------------------------------------------------------------------------------------------------

void CPU::Sti()
{
    asm volatile("sti");
}

// ---------------------------------------------------------------------------------------------------------

void CPU::Hlt()
{
    asm volatile("hlt");
}


} // namespace x86

} // namespace BartOS
