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

void MemoryPool::Initialize()
{
    const PhysicalAddress kernelEndAddr = GetLastUsedMemoryAddress();
    //! Increment the ref counter for every kernel page and remove from free list.
    for (PhysicalPage &physicalPage : Range(m_pPool, m_poolSize))
    {
        if (physicalPage.GetAddress() < kernelEndAddr)
        {
            physicalPage.IncrementRefCount();
            m_freeList.erase(&physicalPage);
        }
        else
        {
            break;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::AddMemoryRegion(const MemoryRegion &memoryRegion)
{
    const uintptr_t lastUsedMemoryAddr = GetLastUsedMemoryAddress();

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

const MemoryPool::PhysicalRange MemoryPool::AllocateRange(const size_t nPages)
{
    // TODO: FINISH THIS, SMH.

    PhysicalPage *pPhysicalPage = m_freeList.front();

    for (; pPhysicalPage < m_freeList.back(); pPhysicalPage = pPhysicalPage->m_freeListHook.next)
    {
        bool isContiguous = false;
        PhysicalPage *pTempPage = pPhysicalPage->m_freeListHook.next;
        for (size_t i = 1; i < nPages - 1; ++i)
        {
            // If the address of the nth page minus (i * PAGE_SIZE) equals the first page, then it's contiguous
            if ((pTempPage->GetAddress() - (i * PAGE_SIZE)) == pPhysicalPage->GetAddress())
            {
                isContiguous = true;
            }
            else
            {
                break;
            }
            
        }

        if (isContiguous)
        {
            PhysicalRange physicalRange;
            physicalRange.m_pPhysicalPage = pPhysicalPage;
            physicalRange.m_nPages = nPages;

            return physicalRange;
        }
    }

    return PhysicalRange();
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::ReturnPage(const PhysicalPage * const pPhysicalPage)
{
    //! Safe to const cast because we know the page came from the pool.
    PhysicalPage *pTempPage = const_cast<PhysicalPage *>(pPhysicalPage);

    pTempPage->DecrementRefCount();
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::ReturnPage(const PhysicalAddress pageAddress)
{
    PhysicalPage *pTempPage = const_cast<PhysicalPage *>(GetPhysicalPage(pageAddress));
    pTempPage->DecrementRefCount();
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::AllocateRange(const PhysicalRange * const pPhysicalRange)
{
    ReturnPage(pPhysicalRange->m_pPhysicalPage);

    uintptr_t pageBaseAddress = pPhysicalRange->m_pPhysicalPage->GetAddress().Get();
    for (size_t i = 1; i < pPhysicalRange->m_nPages; i++)
    {
        ReturnPage(PhysicalAddress(pageBaseAddress + (i * PAGE_SIZE)));
    }
}

// ---------------------------------------------------------------------------------------------------------

const PhysicalPage *MemoryPool::GetPhysicalPage(const PhysicalAddress pageAddr)
{
    // TODO: Optimize with binary search or smth.
    for (PhysicalPage &physicalPage : Range(m_pPool, m_poolSize))
    {
        if (physicalPage.m_addr == pageAddr.PageAddress(PAGE_SIZE))
            return &physicalPage;
    }

    return nullptr;
}

} // namespace MM

} // namespace BartOS
