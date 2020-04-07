#include "CPU.h"
#include "GDT.h"

#include "Libraries/libc/stdio.h"

namespace BartOS
{

namespace x86_64
{

namespace CPU
{

InterruptDisabler::InterruptDisabler()
{
    Cli();
}

// ---------------------------------------------------------------------------------------------------------

InterruptDisabler::~InterruptDisabler()
{
    Sti();
}

// ---------------------------------------------------------------------------------------------------------

void Cli()
{
    asm volatile("cli");
}

// ---------------------------------------------------------------------------------------------------------

void Sti()
{
    asm volatile("sti");
}

// ---------------------------------------------------------------------------------------------------------

void Hlt()
{
    asm volatile("hlt");
}

}

} // namespace x86

} // namespace BartOS
