#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "Kernel/BartOS.h"
#include "Kernel/Arch/x86_64/CPU.h"

#include "Isrs.h"

namespace BartOS
{

inline namespace x86_64
{

namespace Interrupt
{

struct [[gnu::packed]] InterruptContext
{
    // Pushed by the kernel:
    
    uint64_t            m_ds;          ///< The data segment

    //! General purpose registers
    uint64_t            m_r15;
    uint64_t            m_r14;
    uint64_t            m_r13;
    uint64_t            m_r12;
    uint64_t            m_r11;
    uint64_t            m_r10;
    uint64_t            m_r9;
    uint64_t            m_r8;
    uint64_t            m_rbp;
    uint64_t            m_rdi;
    uint64_t            m_rsi;
    uint64_t            m_rdx;
    uint64_t            m_rcx;
    uint64_t            m_rbx;
    uint64_t            m_rax;

    Isrs::InterruptCode m_interruptCode;                ///< The interrupt code.
    uint64_t            m_errorCode;                    ///< The error code.

    // pushed by the CPU:
    uint64_t            m_rip;                          ///< The instruction pointer.
    uint64_t            m_cs;                           ///< The code segment.
    CPU::RFLAGS::Type   m_rflags;                       ///< The rflags.
    uint64_t            m_rsp;                          ///< The stack pointer.
    uint64_t            m_ss;                           ///< The stack segment.
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

class InterruptHandler
{
public:
    /*
     *  @brief  Constructor.
     *
     *  @param  pUserData pointer to user data.
     *  @param  pName the name of the interrupt.
     *  @param  interruptCode the interrupt code.
     */
    InterruptHandler(const void *pUserData, const char *pName, const Isrs::InterruptCode interruptCode);

    //! Destructor.
    virtual ~InterruptHandler();

    /*
     *  @brief  Handle the interrupt
     *
     *  @param  interruptContext the interrupt context.
     *
     *  @retval STATUS_CODE_SUCCESS
     *  @retval ...
     */
    virtual StatusCode Handle(const InterruptContext &interruptContext) const;

    /*
     *  @brief On register callback
     */
    virtual void OnRegister() const;

    /*
     *  @brief On unregister callback
     */
    virtual void OnUnregister() const;

    const void              *m_pUserData;
    const char              *m_pName;
    Isrs::InterruptCode     m_interruptCode;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief  Register an interrupt.
 *
 *  @param  interruptHandler pointer to the interrupt handler;
 */
void RegisterInterrupt(const InterruptHandler *pInterruptHandler);

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief  Handle an interrupt.
 *
 *  @param  interruptContext the interrupt context.
 */
extern "C" void HandleInterrupt(InterruptContext interruptContext);

} // namespace Interrupt

} // namespace x86_64

} // namespace BartOS

#endif // INTERRUPT_H