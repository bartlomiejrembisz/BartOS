#include "Interrupt.h"

#include "Kernel/Arch/x86_64/CPU.h"
#include "Isrs.h"

namespace BartOS
{

namespace x86_64
{

namespace
{

const InterruptHandler *g_pInterruptHandlers[256];

} // namespace

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

InterruptHandler::InterruptHandler(const void *pUserData, const char *pName, const InterruptCode interruptCode) :
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
    kprintf("Handled IRQ %u - %s", GetIrqCode(interruptContext.m_interruptCode), (m_pName) ? m_pName : "");

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
        const InterruptHandler *pInterruptHandler = g_pInterruptHandlers[interruptContext.m_interruptCode];
        if (pInterruptHandler)
        {
            const StatusCode statusCode = pInterruptHandler->Handle(interruptContext);
            if (STATUS_CODE_SUCCESS != statusCode)
                kprintf("Interrupt Handler failed, status code=%u\n", statusCode);
        }
        else
        {
            if (x86_64::EXCEPTION_INVALID_OPCODE == interruptContext.m_interruptCode)
            {
                kprintf("Invalid opcode instruction address: %p\n", interruptContext.m_rip);  
            }
            //kprintf("Undefined IRQ handler %u\n", GetIrqCode(interruptContext.interrupt_num));
        }

        if (!IsException(interruptContext.m_errorCode))
        {
            //! Send an EOI ack to the PICs.
            if (interruptContext.m_interruptCode >= 40)
                out_byte(0xA0, 0x20); // slave
            out_byte(0x20, 0x20);   // master
        }
}

} // namespace x86_64

} // namespace BartOS