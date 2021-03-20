#include "Pmm.h"

#include "Vmm.h"
#include "Multiboot2.h"

#include "Lib/Core/ItemRange.h"


namespace BartOS
{

namespace MM
{

static const size_t kernelSize = ((Address_t)__kernel_physical_start - (Address_t)__kernel_physical_end);
PhysRange Pmm::m_reservedKernelRange((Address_t)__kernel_physical_start, kernelSize);

// ---------------------------------------------------------------------------------------------------------

namespace
{

const char *memoryToStringArray[] = 
{
    "MEMORY_AVAILABLE",
    "MEMORY_RESERVED",
    "MEMORY_ACPI_RECLAIMABLE",
    "MEMORY_NVS",
    "MEMORY_BADRAM",
};

} // namespace

// ---------------------------------------------------------------------------------------------------------

Pmm::Pmm()
{
}

// ---------------------------------------------------------------------------------------------------------

StatusCode Pmm::Initialize(const multiboot_tag_mmap *pMmapTag)
{
    if ((!pMmapTag) && (MULTIBOOT_TAG_TYPE_MMAP == pMmapTag->type))
        return STATUS_CODE_INVALID_PARAMETER;

    kprintf("[PMM] Physical memory map:\n");

    const multiboot_mmap_entry *pMmapEntry;
    for (pMmapEntry = &pMmapTag->entries;
        (const multiboot_uint8_t *) pMmapEntry < (((const multiboot_uint8_t *)pMmapTag) + pMmapTag->size);
        pMmapEntry = (const multiboot_mmap_entry *) (((const multiboot_uint8_t *) pMmapEntry) + pMmapTag->entry_size))
    {
        kprintf("[PMM] Memory region start=%p length=%p type=%s\n", pMmapEntry->addr, pMmapEntry->len, memoryToStringArray[pMmapEntry->type - 1]);

        if (MULTIBOOT_MEMORY_AVAILABLE == pMmapEntry->type)
        {
            CreateZone(pMmapEntry);
        }
    }

    m_freeListSize = m_poolSize;

    set_kmalloc_eternal_ptr(m_pPool + m_poolSize);

    kprintf("[PMM] PMM initialized. Pool start=%p, Page count=%u, Page handle size=%u\n", m_pPool, m_poolSize,
            sizeof(Page));
    kprintf("[PMM] Total system memory: %u MiB\n", GetMemoryStats().m_totalMemory / MiB);
    kprintf("[PMM] Memory used by PMM: %u MiB\n", (sizeof(Page) * m_poolSize) / MiB);

    m_isInitialized = true;

    return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

void Pmm::CreateZone(const multiboot_mmap_entry *pMemoryEntry)
{
    /*
    //! Don't process addresses below the kernel physical start since they're memory mapped to something else.
    PhysAddr pagePhysAddr(NEXT_FRAME_ADDR(memoryRegion.m_addr));

    Page *pPage;
    if(0 == m_poolSize)
    {
        //! Set m_pPool to point right after the kernel text and data.
        pPage = static_cast<Page *>(VirtAddr(get_kmalloc_eternal_ptr()));
        m_pPool = pPage;

        //! Make sure the page for the PhysicalPage object is mapped.
        Vmm::Get().KernelInitMap(PhysAddr::FromVirt(VirtAddr(pPhysicalPage)).PageAddr(PAGE_2M),
                                      VirtAddr(pPhysicalPage).PageAddr(PAGE_2M));
    }
    else
    {
        pPage = &m_pPool[m_poolSize];
    }
    
    const PhysicalAddress regionEnd(PhysicalAddress(memoryRegion.m_addr.Get() + memoryRegion.m_size).PageAddress(PAGE_4K));

    for (; pagePhysAddr < regionEnd; pagePhysAddr += PAGE_SIZE)
    {
        //! Check whether the page for the next PhysicalPage object is mapped.
        Vmm::Get().KernelInitMap(PhysicalAddress::Create(VirtualAddress(pPhysicalPage + 1)).PageAddress(PAGE_2M),
                                      VirtualAddress(pPhysicalPage + 1).PageAddress(PAGE_2M));

        // Placement new to reinitialize.
        new (pPhysicalPage) PhysicalPage(pagePhysAddr);
        ++m_poolSize;
        m_freeList.push_back(pPhysicalPage);

        ++pPhysicalPage;
    }
    */
}

// ---------------------------------------------------------------------------------------------------------

RefPtr<PhysicalPage> Pmm::AllocatePage()
{
    ASSERT(m_freeListSize > 0);

    PhysicalPage *pPhysicalPage = m_freeList.pop_back();
    --m_freeListSize;
    ASSERT(pPhysicalPage);

    return std::move(RefPtr<PhysicalPage>(pPhysicalPage));
}

// ---------------------------------------------------------------------------------------------------------

PhysicalRegion Pmm::AllocateRegion(const size_t nPages)
{
    PhysicalRegion physicalRegion;
    if (0 == nPages)
    {
        ASSERT(false);
        return physicalRegion;
    }
    else if (1 == nPages)
    {
        physicalRegion.m_physicalPageList.push_back(AllocatePage());
        return physicalRegion;
    }

    for (PhysicalPage &physicalPage : ItemRange(m_pPool, m_poolSize))
    {
        PhysicalPage *pFirstPage = &physicalPage;
        PhysicalPage *pLastPage = pFirstPage + nPages;
        bool isContiguous = true;
        for (PhysicalPage *pPhysicalPage = pFirstPage; pPhysicalPage <= pLastPage; ++pPhysicalPage)
        {
            if ((!pPhysicalPage->m_freeListHook.in_list) || (pPhysicalPage->GetRefCount() > 0))
            {
                isContiguous = false;
                break;
            }
        }

        if (isContiguous)
        {
            for (PhysicalPage *pPhysicalPage = pFirstPage; pPhysicalPage <= pLastPage; ++pPhysicalPage)
            {
                m_freeList.erase(pPhysicalPage);
                --m_freeListSize;
                physicalRegion.m_physicalPageList.push_back(pPhysicalPage);
            }
        }
    }

    ASSERT (!physicalRegion.m_physicalPageList.empty());
    return physicalRegion;
}

// ---------------------------------------------------------------------------------------------------------

const PhysicalRegion Pmm::AllocateRegion(const PhysicalAddress physicalAddress, const size_t nPages)
{
    PhysicalRegion physicalRange;
    PhysicalPage *pPhysicalPage = GetPhysicalPage(physicalAddress).Get();
    if (!pPhysicalPage)
    {
        ASSERT(false);
        return physicalRange;
    }

    for (size_t i = 0; (i < nPages) && (pPhysicalPage < (m_pPool + m_poolSize)); ++i)
    {
        physicalRange.m_physicalPageList.push_back(RefPtr<PhysicalPage>(pPhysicalPage));
        ++pPhysicalPage;
        --m_freeListSize;
    }

    return physicalRange;
}

// ---------------------------------------------------------------------------------------------------------

void Pmm::ReturnPage(PhysicalPage * const pPhysicalPage)
{
    m_freeList.push_back(pPhysicalPage);
    ++m_freeListSize;
}

// ---------------------------------------------------------------------------------------------------------

RefPtr<PhysicalPage> Pmm::GetPhysicalPage(const PhysicalAddress pageAddress)
{
    // TODO: Optimize with binary search or smth.
    for (PhysicalPage &physicalPage : ItemRange(m_pPool, m_poolSize))
    {
        if (physicalPage.m_addr == pageAddress.PageAddress(PAGE_SIZE))
            return RefPtr<PhysicalPage>(&physicalPage);
    }

    return RefPtr<PhysicalPage>();
}

// ---------------------------------------------------------------------------------------------------------

Pmm::MemoryStats Pmm::GetMemoryStats()
{
    MemoryStats memoryStats;
    memoryStats.m_totalMemory = m_poolSize * PhysicalPage::m_pageSize;

    //! Calculate the amount of Physical Pages in the free list.
    size_t freeListCounter = 0;
    for (const PhysicalPage &physicalPage : ItemRange(m_pPool, m_poolSize))
    {
        if (physicalPage.m_freeListHook.in_list)
            ++freeListCounter;
    }

    memoryStats.m_usedMemory = (m_poolSize - freeListCounter) * PhysicalPage::m_pageSize;

    return memoryStats;
}

} // namespace MM

} // namespace BartOS