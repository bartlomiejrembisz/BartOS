#ifndef PAGE_FAULT_HANDLER_H
#define PAGE_FAULT_HANDLER_H

#include "Kernel/BartOS.h"

#include "Interrupt.h"

namespace BartOS
{

//! Forward declare the Vmm.
namespace MM { class Vmm; }

inline namespace x86_64
{

namespace Interrupt
{

//! Page fault handler class.
class PageFaultHandler : public InterruptHandler
{
public:
    //! Page fault flags.
    class Flags : public Bitmap<TypeSizeTraits<decltype (InterruptContext::m_errorCode)>::bitSize>
    {
    public:
        typedef BitField<Flags, 1>              Present;
        typedef BitField<Present, 1>            ReadWrite;
        typedef BitField<ReadWrite, 1>          UserSupervisor;
        typedef BitField<UserSupervisor, 29>    Reserved;

        static const Present::Type          PRESENT     = 1;
        static const Present::Type          NOT_PRESENT = 0;
        static const ReadWrite::Type        READ        = 0;
        static const ReadWrite::Type        WRITE       = 1;
        static const UserSupervisor::Type   USER        = 0;
        static const UserSupervisor::Type   SUPERVISOR  = 1;
    };

    /*
     *  @brief Constructor
     * 
     *  @param vmm reference to the vmm.
     */
    PageFaultHandler(MM::Vmm &vmm);

    //! InterruptHandler interface.
    virtual StatusCode Handle(const InterruptContext &interruptContext) const override;
    virtual void OnRegister() const override;
    virtual void OnUnregister() const override;

private:
    MM::Vmm     &m_vmm;
};

} // namespace Interrupt

} // namespace x86_64

} // namespace BartOS

#endif // PAGE_FAULT_HANDLER_H