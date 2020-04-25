#include "MemoryPool.h"

#include "Vmm.h"
#include "Kernel/Multiboot2.h"

#include <new>

namespace BartOS
{

namespace MM
{

namespace
{

PhysicalAddress GetLastUsedMemoryAddress()
{
    PhysicalAddress kernelEndAddr((uintptr_t) __kernel_physical_end);
    PhysicalAddress bootInfoEnd = PhysicalAddress::Create(VirtualAddress(((uintptr_t) g_pBootInfo) + g_pBootInfo->total_size));

    PhysicalAddress lastUsedAddr;
    lastUsedAddr.Set(kernelEndAddr.Get());

    if (bootInfoEnd > lastUsedAddr)
        lastUsedAddr.Set(bootInfoEnd);

    return lastUsedAddr;
}

} // namespace

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

MemoryPool::MemoryPool() :
    m_pPool(nullptr),
    m_poolSize(0)
{
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::AddMemoryRegion(const MemoryRegion &memoryRegion)
{
    const uintptr_t lastUsedMemoryAddr = GetLastUsedMemoryAddress();
    if ((memoryRegion.m_addr.Get() + memoryRegion.m_size) < lastUsedMemoryAddr)
        return;

    PhysicalAddress newPageAddr(ALIGN_TO_NEXT_BOUNDARY(Max(lastUsedMemoryAddr, memoryRegion.m_addr.Get()), PAGE_SIZE));
    VirtualAddress pageObjectAddr;

    if(0 == m_poolSize)
    {
        pageObjectAddr = VirtualAddress::Create(newPageAddr);
        m_pPool = static_cast<PhysicalPage *>(pageObjectAddr);
    }
    else
    {
        pageObjectAddr.Set(reinterpret_cast<uintptr_t>(m_pPool + m_poolSize + 1));
    }

    const PhysicalAddress regionEnd(memoryRegion.m_addr.Get() + memoryRegion.m_size);
    for (; newPageAddr < regionEnd; newPageAddr += PAGE_SIZE)
    {
        PhysicalPage * pPhysicalPage = static_cast<PhysicalPage *>(pageObjectAddr);

        //! Check the page for the next PhysicalPage object is mapped, map it if it isn't.
        if (!Vmm::Get().IsAddressMapped(Vmm::m_pKernelP4Table, pageObjectAddr + sizeof(PhysicalPage)))
        {
            Vmm::Get().MapPage(Vmm::m_pKernelP4Table,
                               PhysicalAddress::Create(pageObjectAddr + sizeof(PhysicalPage)).PageAddress(PAGE_SIZE_2M),
                               (pageObjectAddr + sizeof(PhysicalPage)).PageAddress(PAGE_SIZE_2M) ,
                               static_cast<PageFlags>(PRESENT | WRITABLE | HUGE_PAGE), PAGE_2M);
        }

        // Placement to reinitialize.
        new (pPhysicalPage) PhysicalPage(newPageAddr);
        ++m_poolSize;
        m_freeList.push_back(pPhysicalPage);

        pageObjectAddr += sizeof(PhysicalPage);
    }
    
}

// ---------------------------------------------------------------------------------------------------------

const PhysicalPage *MemoryPool::AllocatePage()
{
    PhysicalPage *pPhysicalPage = m_freeList.pop_back();
    ASSERT(pPhysicalPage);

    pPhysicalPage->IncrementRefCount();

    return pPhysicalPage;
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::ReturnPage(const PhysicalPage * const pPhysicalPage)
{
    //! Safe to const cast because we know the page came from the pool.
    PhysicalPage *pTempPage = const_cast<PhysicalPage *>(pPhysicalPage);

    pTempPage->DecrementRefCount();

    m_freeList.push_back(pTempPage);
}

// ---------------------------------------------------------------------------------------------------------

const RefPtr<PhysicalPage> MemoryPool::GetPhysicalPage(const uintptr_t pageAddr)
{
    for (PhysicalPage &physicalPage : Range(m_pPool, m_poolSize))
    {
        if (physicalPage.m_addr.Get() == pageAddr)
            return RefPtr<PhysicalPage>(&physicalPage);
    }

    return RefPtr<PhysicalPage>();
}

} // namespace MM

} // namespace BartOS
