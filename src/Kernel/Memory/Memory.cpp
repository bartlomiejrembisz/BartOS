#include "Memory.h"
#include "Vmm.h"

#include "Kernel/Multiboot2.h"

#define ALIGNMENT_BYTES 8

namespace BartOS
{

namespace
{

uint8_t *pEternalMallocPointer;

}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

void init_kmalloc_eternal()
{
    // Initialize pointer to kernel end.
    pEternalMallocPointer  = reinterpret_cast<uint8_t *> (ALIGN_TO_NEXT_BOUNDARY(reinterpret_cast<uintptr_t>(&__kernel_virtual_end), ALIGNMENT_BYTES));

    uint8_t *pBootInfoStruct = static_cast<uint8_t *>(VirtualAddress(((uintptr_t) g_pBootInfo) + g_pBootInfo->total_size));

    if (pBootInfoStruct > pEternalMallocPointer)
        pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((uintptr_t) pBootInfoStruct, ALIGNMENT_BYTES));
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc_eternal(const size_t size)
{
    uint8_t *pAlloc = pEternalMallocPointer;
    pEternalMallocPointer += size;
    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((uintptr_t)pEternalMallocPointer, ALIGNMENT_BYTES));

    //! Make sure every page for the allocation is mapped.
    for (uint8_t *pAllocation = pAlloc; pAllocation < (pAlloc + size); pAllocation += PAGE_SIZE_2M)
    {
        uint8_t *pMappingMemory = pAllocation;
        //! HACK: Overflow guard.
        if (pAllocation < pAlloc)
            break;

        MM::Vmm::Get().EnsureKernelMapped(PhysicalAddress::Create(VirtualAddress(pMappingMemory)).PageAddress(PAGE_SIZE_2M),
                                    VirtualAddress(pMappingMemory).PageAddress(PAGE_SIZE_2M),
                                    static_cast<PageFlags>(PRESENT | WRITABLE | HUGE_PAGE), PAGE_2M);
    }

    return reinterpret_cast<void *>(pAlloc);
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc_eternal_aligned(const size_t size, size_t alignment)
{
    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((uintptr_t) pEternalMallocPointer, alignment));
    return kmalloc_eternal(size);
}

// ---------------------------------------------------------------------------------------------------------

void set_kmalloc_eternal_ptr(void *pKmallocEternal)
{
    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((uintptr_t) pKmallocEternal, ALIGNMENT_BYTES));
}

// ---------------------------------------------------------------------------------------------------------

void *get_kmalloc_eternal_ptr()
{
    return static_cast<void *>(pEternalMallocPointer);
}

} // namespace BartOS
