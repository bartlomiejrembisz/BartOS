#include "Interrupt.h"

#include "Kernel/Arch/x86_64/CPU.h"
#include "Isrs.h"

namespace BartOS
{

inline namespace x86_64
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
    kprintf("Handled IRQ %u - %s", Isrs::GetIrqCode(interruptContext.m_interruptCode), (m_pName) ? m_pName : "");

    return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------


void InterruptHandler::OnRegister() const
{
}

// ---------------------------------------------------------------------------------------------------------

void InterruptHandler::OnUnregister() const
{
}

// ---------------------------------------------------------------------------------------------------------
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
    if (Isrs::IsException(interruptContext.m_interruptCode))
    {
        kprintf("Interrupt No: %u - %s\nInstruction address = %p\n", interruptContext.m_interruptCode,
                Isrs::GetIsrMnemonic(interruptContext.m_interruptCode), interruptContext.m_rip);

        if (interruptContext.m_interruptCode == Isrs::EXCEPTION_PAGE_FAULT)
        {
            kprintf("Page Fault Address: %p\n", CPU::GetCR2());
        }

        if (Isrs::EXCEPTION_INVALID_OPCODE == interruptContext.m_interruptCode)
            CPU::Hlt();
    }
    else // IRQ
    {
        const InterruptHandler *pInterruptHandler = g_pInterruptHandlers[Isrs::GetIrqCode(interruptContext.m_interruptCode)];
        if (pInterruptHandler)
        {
            const StatusCode statusCode = pInterruptHandler->Handle(interruptContext);
            if (STATUS_CODE_SUCCESS != statusCode)
                kprintf("Interrupt Handler failed, status code=%u\n", statusCode);
        }
        else
        {
            //kprintf("Undefined IRQ handler %u\n", Isrs::GetIrqCode(interruptContext.interrupt_num));
        }

        //! Send an EOI ack to the PICs.
        if (interruptContext.m_interruptCode >= 40)
            out_byte(0xA0, 0x20); // slave
        out_byte(0x20, 0x20);   // master
    }
}

} // namespace Interrupt

} // namespace x86_64

} // namespace BartOS