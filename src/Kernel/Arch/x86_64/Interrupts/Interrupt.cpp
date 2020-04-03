#include "Interrupt.h"

#include "Isrs.h"

namespace BartOS
{

namespace x86_64
{

namespace Interrupt
{

extern "C" void InterruptHandler(InterruptContext interruptContext)
{
    kprintf("Interrupt No: %u - %s\nInstruction address = %p", interruptContext.interrupt_num,
            Isrs::GetIsrMnemonic(interruptContext.interrupt_num), interruptContext.rip);
}

} // namespace Interrupt

} // namespace x86_64

} // namespace BartOS