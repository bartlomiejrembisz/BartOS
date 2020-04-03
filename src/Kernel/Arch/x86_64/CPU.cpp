#include "CPU.h"
#include "GDT.h"

#include "Libraries/libc/stdio.h"

namespace BartOS
{

namespace x86_64
{

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
