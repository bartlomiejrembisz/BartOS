#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "Kernel/BartOS.h"
#include "Isrs.h"

namespace BartOS
{

namespace x86_64
{

namespace Interrupt
{

struct [[gnu::packed]] InterruptContext
{
    // pushed by the kernel:
    // The data segment
    uint64_t ds;

    // general purpose registers
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    // the interrupt number.
    uint64_t interrupt_num;

    // error code.
    uint64_t error_code;

    // pushed by the CPU:
    // saved instruction pointer.
    uint64_t rip;

    // saved code segment.
    uint64_t cs;

    // rflags
    uint64_t rflags;

    // stack pointer and stack segment
    uint64_t rsp;
    uint64_t ss;
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
     *
     *  @retval STATUS_CODE_SUCCESS;
     *  @retval ...
     */
    virtual StatusCode OnRegister() const;

    /*
     *  @brief On unregister callback
     *
     *  @retval STATUS_CODE_SUCCESS;
     *  @retval ...
     */
    virtual StatusCode OnUnregister() const;

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