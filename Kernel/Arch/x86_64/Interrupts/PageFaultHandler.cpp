#include "PageFaultHandler.h"

#include "Memory/Vmm.h"

namespace BartOS
{

namespace x86_64
{

PageFaultHandler::PageFaultHandler(MM::Vmm &vmm) :
    InterruptHandler(nullptr, "PageFaultHandler", EXCEPTION_PAGE_FAULT),
    m_vmm(vmm)
{
}

// ---------------------------------------------------------------------------------------------------------

StatusCode PageFaultHandler::Handle(const InterruptContext &interruptContext) const
{
    const VirtAddr vAddrFault(GetCR2());
    const Flags &pageFaultFlags(reinterpret_cast<const Flags &>(interruptContext.m_errorCode));

    kprintf("Page Fault Address: %p\n", vAddrFault.Get());
    kprintf("Instruction Address: %p\n", interruptContext.m_rip);

    return STATUS_CODE_SUCCESS;

    MM::VmArea *pVmArea   = nullptr;
    MM::AddressSpace *pAddressSpace = nullptr;

    const bool isKernelAddress      = true; //MM::AddressSpace::IsKernelAddress(vAddrFault);

    const bool isProtectionFault    = (pageFaultFlags.Get<Flags::Present>() == Flags::PRESENT);
    const bool isNonPresentPage     = (pageFaultFlags.Get<Flags::Present>() == Flags::NOT_PRESENT);
    const bool isRead               = (pageFaultFlags.Get<Flags::ReadWrite>() == Flags::READ);
    const bool isWrite              = (pageFaultFlags.Get<Flags::ReadWrite>() == Flags::WRITE);
    const bool isSupervisor         = (pageFaultFlags.Get<Flags::UserSupervisor>() == Flags::SUPERVISOR);
    const bool isUser               = (pageFaultFlags.Get<Flags::UserSupervisor>() == Flags::SUPERVISOR);

    //! Page fault occured in the kernel address space.
    if (isKernelAddress)
    {
        if (isUser)
        {
            // SEGFAULT.
        }
        
        //pVmArea = m_vmm.m_kernelAddressSpace.GetVmArea(vAddrFault);
        //pAddressSpace = &m_vmm.m_kernelAddressSpace;
    }
    else
    {
        // Get user Address Space.
    }
    
    ASSERT(pVmArea && pAddressSpace);

    if (isProtectionFault)
    {
        if (isUser)
        {
            // User tried accessing kernel data.
            // SEGFAULT.
        }
        else
        {
            // Kernel tried to read or write a page and caused a protection fault.
        }
    }
    else
    {
        //! Page not present.
        //if (pVmArea->GetFlags() & ALLOCATE_ON_DEMAND)
        {
            // Allocate the page regardless whether it was a user or not.
            //pAddressSpace->AllocateOnDemand(*pVmArea);
        }
    }

    return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

void PageFaultHandler::OnRegister() const
{
    kprintf("PageFaultHandler registered.\n");
}

// ---------------------------------------------------------------------------------------------------------

void PageFaultHandler::OnUnregister() const
{
    kprintf("PageFaultHandler unregistered\n");
    ASSERT(false);
}

} // namespace x86_64

} // namespace BartOS