#include "Interrupt.h"

#include "Kernel/Arch/x86_64/CPU.h"

namespace BartOS
{

namespace x86_64
{

namespace Interrupt
{

namespace
{

const InterruptHandler *g_pInterruptHandlers[256];

} // namespace

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

InterruptHandler::InterruptHandler(const void *pUserData, const char *pName, const Isrs::InterruptCode interruptCode) :
    m_pUserData(pUserData),
    m_pName(pName),
    m_interruptCode(interruptCode)
{
}

// ---------------------------------------------------------------------------------------------------------

InterruptHandler::~InterruptHandler()
{
}

// ---------------------------------------------------------------------------------------------------------

StatusCode InterruptHandler::Handle(const InterruptContext &interruptContext) const
{
    kprintf("Handled IRQ %u - %s", Isrs::GetIrqCode(interruptContext.interrupt_num), (m_pName) ? m_pName : "");
}

// ---------------------------------------------------------------------------------------------------------


StatusCode InterruptHandler::OnRegister() const
{
}

// ---------------------------------------------------------------------------------------------------------

StatusCode InterruptHandler::OnUnregister() const
{
}

// ---------------------------------------------------------------------------------------------------------

void RegisterInterrupt(const InterruptHandler *pInterruptHandler)
{
    if (pInterruptHandler)
    {
        const InterruptHandler *pCurrentInterruptHandler = g_pInterruptHandlers[pInterruptHandler->m_interruptCode];
        if (pCurrentInterruptHandler)
            pCurrentInterruptHandler->OnUnregister();

        //! m_interruptCode is the IRQ code.
        g_pInterruptHandlers[pInterruptHandler->m_interruptCode] = pInterruptHandler;
        pInterruptHandler->OnRegister();
    }
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

extern "C" void HandleInterrupt(InterruptContext interruptContext)
{
    if (Isrs::IsException(interruptContext.interrupt_num))
    {
        kprintf("Interrupt No: %u - %s\nInstruction address = %p\n", interruptContext.interrupt_num,
                Isrs::GetIsrMnemonic(interruptContext.interrupt_num), interruptContext.rip);

        if (Isrs::EXCEPTION_INVALID_OPCODE == interruptContext.interrupt_num)
            CPU::Hlt();
    }
    else // IRQ
    {
        kprintf("IRQ %u received\n", Isrs::GetIrqCode(interruptContext.interrupt_num));

        //! Send an EOI ack to the PICs.
        if (interruptContext.interrupt_num >= 40) out_byte(0xA0, 0x20); // slave
        out_byte(0x20, 0x20);   // master

        const InterruptHandler *pInterruptHandler = g_pInterruptHandlers[Isrs::GetIrqCode(interruptContext.interrupt_num)];
        if (pInterruptHandler)
            pInterruptHandler->Handle(interruptContext);
        else
            kprintf("Undefined IRQ handler %u", Isrs::GetIrqCode(interruptContext.interrupt_num));
    }


}

} // namespace Interrupt

} // namespace x86_64

} // namespace BartOS