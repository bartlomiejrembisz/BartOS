#include "Pmm.h"
#include "Kernel/Multiboot2.h"

#include "frg/list.hpp"

namespace BartOS
{

namespace MM
{

namespace
{

const char * memoryToStringArray[] = 
{
    "MEMORY_AVAILABLE",
    "MEMORY_RESERVED",
    "MEMORY_ACPI_RECLAIMABLE",
    "MEMORY_NVS",
    "MEMORY_BADRAM",
};

} // namespace

// ---------------------------------------------------------------------------------------------------------

Pmm::Pmm() :
    m_isInitialized(false)
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
            MemoryPool::MemoryRegion memoryRegion;
            memoryRegion.m_addr = PhysicalAddress(static_cast<uintptr_t>(pMmapEntry->addr));
            memoryRegion.m_size = pMmapEntry->len;

            m_memoryPool.AddMemoryRegion(memoryRegion);
        }
    }

    set_kmalloc_eternal_ptr(m_memoryPool.m_pPool + m_memoryPool.m_poolSize + 1);

    kprintf("[PMM] PMM initialized. Pool start=%p, Page count=%u, Page handle size=%u\n", m_memoryPool.m_pPool, m_memoryPool.m_poolSize,
            sizeof(PhysicalPage));
    kprintf("[PMM] Total system memory: %u MiB\n", GetMemoryStats().m_totalMemory / MiB);
    kprintf("[PMM] Memory used by PMM: %u MiB\n", (sizeof(PhysicalPage) * m_memoryPool.m_poolSize) / MiB);

    m_isInitialized = true;

    return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

const PhysicalPage *Pmm::AllocatePage()
{
    return m_memoryPool.AllocatePage();
}

// ---------------------------------------------------------------------------------------------------------

const Pmm::PhysicalRange Pmm::AllocateRange(const size_t nPages)
{
    return m_memoryPool.AllocateRange(nPages);
}

// ---------------------------------------------------------------------------------------------------------

const Pmm::PhysicalRange Pmm::AllocateRange(const PhysicalAddress physicalAddress, const size_t nPages)
{
    return m_memoryPool.AllocateRange(physicalAddress, nPages);
}

// ---------------------------------------------------------------------------------------------------------

void Pmm::ReturnPage(const PhysicalPage * const pPhysicalPage)
{
    m_memoryPool.ReturnPage(pPhysicalPage);
}

// ---------------------------------------------------------------------------------------------------------

void Pmm::ReturnRange(PhysicalRange &physicalRange)
{
    m_memoryPool.ReturnRange(physicalRange);
}

// ---------------------------------------------------------------------------------------------------------

Pmm::MemoryStats Pmm::GetMemoryStats()
{
    MemoryStats memoryStats;
    memoryStats.m_totalMemory = m_memoryPool.m_poolSize * PhysicalPage::m_pageSize;

    //! Calculate the amount of Physical Pages in the free list.
    size_t freeListCounter = 0;
    for (const PhysicalPage &physicalPage : Range(m_memoryPool.m_pPool, m_memoryPool.m_poolSize))
    {
        if (physicalPage.m_freeListHook.in_list)
            ++freeListCounter;
    }

    memoryStats.m_usedMemory = (m_memoryPool.m_poolSize - freeListCounter) * PhysicalPage::m_pageSize;

    return memoryStats;
}

// ---------------------------------------------------------------------------------------------------------

PhysicalAddress Pmm::GetEndAddress()
{
    if (!m_isInitialized)
        return PhysicalAddress(0);

    return PhysicalAddress::Create(VirtualAddress(reinterpret_cast<intptr_t>(m_memoryPool.m_pPool + m_memoryPool.m_poolSize)));
}

// ---------------------------------------------------------------------------------------------------------

void Pmm::InitializePhysicalRange(PhysicalRange &physicalRange)
{
    m_memoryPool.InitializePhysicalRange(physicalRange);
}

} // namespace MM

} // namespace BartOS