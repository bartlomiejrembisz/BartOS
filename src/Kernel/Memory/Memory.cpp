#include "Memory.h"
#include "Vmm.h"

#include "Kernel/Multiboot2.h"

#define ALIGNMENT_BYTES 8

namespace BartOS
{

namespace
{

uint8_t *pEternalMallocPointer = nullptr;

bool isMallocEternalEnabled = false;

}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

void init_kmalloc_eternal()
{
    ASSERT((!isMallocEternalEnabled) && (!isMallocEternalEnabled));

    // Initialize pointer to kernel end.
    pEternalMallocPointer  = reinterpret_cast<uint8_t *> (ALIGN_TO_NEXT_BOUNDARY(reinterpret_cast<Address_t>(&__kernel_virtual_end), ALIGNMENT_BYTES));

    uint8_t *pBootInfoStruct = static_cast<uint8_t *>(VirtualAddress(((Address_t) g_pBootInfo) + g_pBootInfo->total_size));

    if (pBootInfoStruct > pEternalMallocPointer)
        pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t) pBootInfoStruct, ALIGNMENT_BYTES));

    isMallocEternalEnabled = true;
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc_eternal(const size_t size)
{
    ASSERT(isMallocEternalEnabled);

    uint8_t *pAlloc = pEternalMallocPointer;
    pEternalMallocPointer += size;
    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t)pEternalMallocPointer, ALIGNMENT_BYTES));

    //! Make sure every page for the allocation is mapped.
    for (uint8_t *pAllocation = pAlloc; pAllocation < (pAlloc + size); pAllocation += PAGE_2M)
    {
        uint8_t *pMappingMemory = pAllocation;
        //! HACK: Overflow guard.
        if (pAllocation < pAlloc)
            break;

        MM::Vmm::Get().EnsureKernelMapped(PhysicalAddress::Create(VirtualAddress(pMappingMemory)).PageAddress(PAGE_2M),
                                    VirtualAddress(pMappingMemory).PageAddress(PAGE_2M),
                                    (PRESENT | WRITABLE | HUGE_PAGE), PAGE_2M);
    }

    return reinterpret_cast<void *>(pAlloc);
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc_eternal_aligned(const size_t size, size_t alignment)
{
    ASSERT(isMallocEternalEnabled);

    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t) pEternalMallocPointer, alignment));
    return kmalloc_eternal(size);
}

// ---------------------------------------------------------------------------------------------------------

void set_kmalloc_eternal_ptr(void *pKmallocEternal)
{
    ASSERT(isMallocEternalEnabled);

    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t) pKmallocEternal, ALIGNMENT_BYTES));
}

// ---------------------------------------------------------------------------------------------------------

void *get_kmalloc_eternal_ptr()
{
    ASSERT(isMallocEternalEnabled);

    return static_cast<void *>(pEternalMallocPointer);
}

// ---------------------------------------------------------------------------------------------------------

void disable_kmalloc_eternal()
{
    ASSERT(isMallocEternalEnabled);
    
    isMallocEternalEnabled = false;
}

} // namespace BartOS

void *operator new(size_t size)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Allocate(size);
}

// ---------------------------------------------------------------------------------------------------------

void *operator new[](size_t size)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Allocate(size);
}

// ---------------------------------------------------------------------------------------------------------

void operator delete(void* ptr)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Free(ptr);
}

// ---------------------------------------------------------------------------------------------------------

void operator delete[](void* ptr)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Free(ptr);
}

// ---------------------------------------------------------------------------------------------------------

void operator delete(void *ptr, size_t size)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Free(ptr);
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc(size_t size)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Allocate(size);
}

// ---------------------------------------------------------------------------------------------------------

void kfree(void *ptr)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Free(ptr);
}