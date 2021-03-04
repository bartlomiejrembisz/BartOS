#include "Memory.h"
#include "Vmm.h"

#include "Kernel/Multiboot2.h"

#define ALIGNMENT_BYTES 8

namespace BartOS
{

void *Allocator::allocate(const size_t size)
{
    return kmalloc(size);
}

// ---------------------------------------------------------------------------------------------------------

void Allocator::free(void *ptr)
{
    kfree(ptr);
}

// ---------------------------------------------------------------------------------------------------------

void Allocator::deallocate(void *ptr, unsigned long size)
{
    (void)size;
    kfree(ptr);
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

namespace
{

uint8_t *pEternalMallocPointer = nullptr;
uint8_t *pEternalMallocBegin = nullptr;

bool isMallocEternalEnabled = false;

}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

void init_kmalloc_eternal()
{
    ASSERT((!isMallocEternalEnabled) && (!pEternalMallocPointer) && (!pEternalMallocBegin));

    // Initialize pointer to kernel end.
    pEternalMallocPointer  = reinterpret_cast<uint8_t *> (ALIGN_TO_NEXT_BOUNDARY(reinterpret_cast<Address_t>(&__kernel_virtual_end), PAGE_SIZE));

    uint8_t *pBootInfoStruct = static_cast<uint8_t *>(VirtualAddress(((Address_t) g_pBootInfo) + g_pBootInfo->total_size));

    if (pBootInfoStruct > pEternalMallocPointer)
        pEternalMallocPointer = reinterpret_cast<uint8_t *>((Address_t) pBootInfoStruct);

    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t) pEternalMallocPointer, PAGE_SIZE));

    isMallocEternalEnabled = true;
    pEternalMallocBegin = pEternalMallocPointer;
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

        MM::Vmm::Get().KernelInitMap(PhysicalAddress::Create(VirtualAddress(pMappingMemory)).PageAddress(PAGE_2M),
                                    VirtualAddress(pMappingMemory).PageAddress(PAGE_2M));
    }

    return reinterpret_cast<void *>(pAlloc);
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc_eternal_aligned(const size_t size, size_t alignment)
{
    ASSERT(isMallocEternalEnabled);

    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t) pEternalMallocPointer, alignment));
    MM::Vmm::Get().KernelInitMap(PhysicalAddress::Create(VirtualAddress(pEternalMallocPointer)).PageAddress(PAGE_2M),
                                    VirtualAddress(pEternalMallocPointer).PageAddress(PAGE_2M));
                                    
    return kmalloc_eternal(size);
}

// ---------------------------------------------------------------------------------------------------------

void set_kmalloc_eternal_ptr(void *pKmallocEternal)
{
    ASSERT(isMallocEternalEnabled);

    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t) pKmallocEternal, ALIGNMENT_BYTES));
}

// ---------------------------------------------------------------------------------------------------------

void *get_kmalloc_eternal_begin()
{
    return static_cast<void *>(pEternalMallocBegin);
}

// ---------------------------------------------------------------------------------------------------------

void *get_kmalloc_eternal_ptr()
{
    return static_cast<void *>(pEternalMallocPointer);
}

// ---------------------------------------------------------------------------------------------------------

void disable_kmalloc_eternal()
{
    ASSERT(isMallocEternalEnabled);
    
    isMallocEternalEnabled = false;

    pEternalMallocPointer = reinterpret_cast<uint8_t *>(ALIGN_TO_NEXT_BOUNDARY((Address_t) pEternalMallocPointer, PAGE_SIZE));
}

} // namespace BartOS

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

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
    (void)size;
    return BartOS::MM::Vmm::Get().GetKernelHeap().Free(ptr);
}

// ---------------------------------------------------------------------------------------------------------

void operator delete[](void *ptr, size_t size)
{
    (void)size;
    return BartOS::MM::Vmm::Get().GetKernelHeap().Free(ptr);
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc(const size_t size)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Allocate(size);
}

// ---------------------------------------------------------------------------------------------------------

void *kmalloc(const size_t size, const size_t alignment)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Allocate(size, alignment);
}

// ---------------------------------------------------------------------------------------------------------

void kfree(void * const ptr)
{
    return BartOS::MM::Vmm::Get().GetKernelHeap().Free(ptr);
}
