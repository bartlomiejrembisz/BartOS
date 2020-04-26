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

Pmm::Pmm()
{
}

// ---------------------------------------------------------------------------------------------------------

StatusCode Pmm::Initialize(const multiboot_tag_mmap *pMmapTag)
{
    if ((!pMmapTag) && (MULTIBOOT_TAG_TYPE_MMAP == pMmapTag->type))
        return STATUS_CODE_INVALID_PARAMETER;

    kprintf("\nPMM:\n");
    kprintf("Physical memory map:\n");

    const multiboot_mmap_entry *pMmapEntry;
    for (pMmapEntry = &pMmapTag->entries;
        (const multiboot_uint8_t *) pMmapEntry < (((const multiboot_uint8_t *)pMmapTag) + pMmapTag->size);
        pMmapEntry = (const multiboot_mmap_entry *) (((const multiboot_uint8_t *) pMmapEntry) + pMmapTag->entry_size))
    {
        kprintf("Memory region start=%p length=%p type=%s\n", pMmapEntry->addr, pMmapEntry->len, memoryToStringArray[pMmapEntry->type - 1]);

        if (MULTIBOOT_MEMORY_AVAILABLE == pMmapEntry->type)
        {
            MemoryPool::MemoryRegion memoryRegion;
            memoryRegion.m_addr = PhysicalAddress(static_cast<uintptr_t>(pMmapEntry->addr));
            memoryRegion.m_size = pMmapEntry->len;

            m_memoryPool.AddMemoryRegion(memoryRegion);
        }
    }

    kprintf("PMM initialized. Pool start=%p, Page count=%u, Page handle size=%u\n", m_memoryPool.m_pPool, m_memoryPool.m_poolSize,
            sizeof(PhysicalPage));
    kprintf("Memory used by PMM: %u MiB\n", (sizeof(PhysicalPage) * m_memoryPool.m_poolSize) / MiB);

    return STATUS_CODE_SUCCESS;
}

} // namespace MM

} // namespace BartOS