#include "MemoryPool.h"

#include "Vmm.h"
#include "Pmm.h"

#include "KernelAddressSpace.h"

#include <new>

namespace BartOS
{

namespace MM
{

MemoryPool::MemoryPool() :
    m_pPool(nullptr),
    m_poolSize(0)
{
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::AddMemoryRegion(const MemoryRegion &memoryRegion)
{
    //! Don't process addresses below the kernel physical start since they're memory mapped to something else.
    PhysicalAddress pagePhysAddr(ALIGN_TO_NEXT_BOUNDARY(memoryRegion.m_addr.Get(), PAGE_SIZE));

    PhysicalPage *pPhysicalPage;
    if(0 == m_poolSize)
    {
        //! Set m_pPool to point right after the kernel text and data.
        pPhysicalPage = static_cast<PhysicalPage *>(VirtualAddress(get_kmalloc_eternal_ptr()));
        m_pPool = pPhysicalPage;

        //! Make sure the page for the PhysicalPage object is mapped.
        Vmm::Get().EnsureKernelMapped(PhysicalAddress::Create(VirtualAddress(pPhysicalPage)).PageAddress(PAGE_2M),
                                      VirtualAddress(pPhysicalPage).PageAddress(PAGE_2M),
                                      (PRESENT | WRITABLE | HUGE_PAGE), PAGE_2M);
    }
    else
    {
        pPhysicalPage = &m_pPool[m_poolSize];
    }
    
    const PhysicalAddress regionEnd(memoryRegion.m_addr.Get() + memoryRegion.m_size);

    for (; pagePhysAddr < regionEnd; pagePhysAddr += PAGE_SIZE)
    {
        //! Check whether the page for the next PhysicalPage object is mapped.
        Vmm::Get().EnsureKernelMapped(PhysicalAddress::Create(VirtualAddress(pPhysicalPage + 1)).PageAddress(PAGE_2M),
                                      VirtualAddress(pPhysicalPage + 1).PageAddress(PAGE_2M),
                                      (PRESENT | WRITABLE | HUGE_PAGE), PAGE_2M);

        // Placement new to reinitialize.
        new (pPhysicalPage) PhysicalPage(pagePhysAddr);
        ++m_poolSize;
        m_freeList.push_back(pPhysicalPage);

        ++pPhysicalPage;
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

MemoryPool::PhysicalRange MemoryPool::AllocateRange(const size_t nPages)
{
    for (PhysicalPage &physicalPage : Range(m_pPool, m_poolSize))
    {
        bool isContiguous = false;
        for (size_t nPage = 0; nPage < nPages; ++nPage)
        {
            PhysicalPage *pPhysicalPage = &physicalPage + nPage;
            if (pPhysicalPage->m_freeListHook.in_list)
            {
                isContiguous = false;
                break;
            }
            else
            {
                isContiguous = true;
            }
        }

        if (isContiguous)
        {
            PhysicalRange physicalRange(&physicalPage, nPages);

            return physicalRange;
        }
    }

    return PhysicalRange(nullptr, 0);
}

// ---------------------------------------------------------------------------------------------------------

MemoryPool::PhysicalRange MemoryPool::AllocateRange(const PhysicalAddress physicalAddress, const size_t nPages)
{
    PhysicalPage * const pPhysicalPage = const_cast<PhysicalPage *>(FindPhysicalPage(physicalAddress));

    PhysicalRange physicalRange(pPhysicalPage, nPages);

    return physicalRange;
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::InitializePhysicalRange(PhysicalRange &physicalRange)
{
    //! Safe to const cast because we know the page came from the pool.
    for (const PhysicalPage &physicalPage : Range(physicalRange.m_pPhysicalPage, physicalRange.m_nPages))
    {
        PhysicalPage &tempPageReference = const_cast<PhysicalPage &>(physicalPage);
        tempPageReference.IncrementRefCount();
        if (tempPageReference.m_freeListHook.in_list)
            m_freeList.erase(&tempPageReference);
    }
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
    //! Safe to const cast because we know the page came from the pool.
    PhysicalPage *pTempPage = const_cast<PhysicalPage *>(FindPhysicalPage(pageAddress));

    pTempPage->DecrementRefCount();
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::ReturnRange(MemoryPool::PhysicalRange &physicalRange)
{
    for (const PhysicalPage &physicalPage : Range(physicalRange.m_pPhysicalPage, physicalRange.m_nPages))
        ReturnPage(&physicalPage);

    physicalRange.Clear();
}

// ---------------------------------------------------------------------------------------------------------

const PhysicalPage *MemoryPool::FindPhysicalPage(const PhysicalAddress pageAddress)
{
    // TODO: Optimize with binary search or smth.
    for (PhysicalPage &physicalPage : Range(m_pPool, m_poolSize))
    {
        if (physicalPage.m_addr == pageAddress.PageAddress(PAGE_SIZE))
            return &physicalPage;
    }

    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------

const RefPtr<PhysicalPage> MemoryPool::GetPhysicalPage(const PhysicalAddress pageAddress)
{
    //! Safe to const cast because we know the page came from the pool.
    return RefPtr<PhysicalPage>(const_cast<PhysicalPage *>(FindPhysicalPage(pageAddress)));
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

MemoryPool::PhysicalRange::PhysicalRange(PhysicalPage * const pPhysicalPage, const size_t nPages) :
    m_pPhysicalPage(pPhysicalPage),
    m_nPages(nPages)
{
    if (IsInitalized())
        Pmm::Get().InitializePhysicalRange(*this);
}

// ---------------------------------------------------------------------------------------------------------

MemoryPool::PhysicalRange::PhysicalRange(const PhysicalRange &rhs) :
    m_pPhysicalPage(rhs.m_pPhysicalPage),
    m_nPages(rhs.m_nPages)
{
    if (IsInitalized())
        Pmm::Get().InitializePhysicalRange(*this);
}

// ---------------------------------------------------------------------------------------------------------

MemoryPool::PhysicalRange::PhysicalRange(PhysicalRange &&rhs) :
    m_pPhysicalPage(rhs.m_pPhysicalPage),
    m_nPages(rhs.m_nPages)
{
    rhs.Clear();
}

// ---------------------------------------------------------------------------------------------------------

MemoryPool::PhysicalRange &MemoryPool::PhysicalRange::operator=(const PhysicalRange &rhs)
{
    //! If the object is currently initialized then return to pool
    ReturnToPool();
    
    m_pPhysicalPage = rhs.m_pPhysicalPage;
    m_nPages = rhs.m_nPages;

    if (IsInitalized())
        Pmm::Get().InitializePhysicalRange(*this);

    return *this;
}

// ---------------------------------------------------------------------------------------------------------

MemoryPool::PhysicalRange &MemoryPool::PhysicalRange::operator=(PhysicalRange &&rhs)
{
    //! If the object is currently initialized then return to pool
    ReturnToPool();
    
    m_pPhysicalPage = rhs.m_pPhysicalPage;
    m_nPages = rhs.m_nPages;

    rhs.Clear();

    return *this;
}

// ---------------------------------------------------------------------------------------------------------

void MemoryPool::PhysicalRange::ReturnToPool()
{
    if (IsInitalized())
        Pmm::Get().ReturnRange(*this);

    Clear();
}

} // namespace MM

} // namespace BartOS
