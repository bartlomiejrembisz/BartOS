#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include "BartOS.h"

#include "SlabAllocator.h"
#include "Kernel/Arch/x86_64/Paging/PageTable.h"

namespace BartOS
{

namespace MM
{

class Vmm;

//! Kernel heap manager object.
class KernelHeap
{
public:
    //! Constructor
    KernelHeap();

    //! Initialize the Kernel Heap.
    void Initialize();

    /*
     *  @brief Allocate a buffer.
     * 
     *  @param nBytes the amount of bytes to allocate.
     * 
     *  @return pointer to the allocated buffer.
     */
    void *Allocate(const size_t nBytes);

    /*
     *  @brief Allocate an aligned buffer.
     * 
     *  @param nBytes the amount of bytes to allocate.
     *  @oaram alignment the alignment.
     * 
     *  @return pointer to the allocated buffer.
     */
    void *Allocate(const size_t nBytes, const size_t alignment);

    /*
     *  @brief Free a buffer.
     * 
     *  @param pBuffer pointer to the buffer to be freed.
     */
    void Free(void *pBuffer);

    /*
     *  @brief Allocate a PageTable.
     * 
     *  @return pointer to the page table.
     */
    x86_64::PageTable *AllocatePml();

    /*
     *  @brief Allocate a PageTable.
     * 
     *  @param pPageTable pointer to the page table.
     */
    void FreePml(x86_64::PageTable * const pPageTable);

private:
    SlabAllocator<16>       m_slab16;                       ///< The 16 byte slab allocator.
    SlabAllocator<32>       m_slab32;                       ///< The 32 byte slab allocator.
    SlabAllocator<64>       m_slab64;                       ///< The 64 byte slab allocator.
    SlabAllocator<128>      m_slab128;                      ///< The 128 byte slab allocator.
    SlabAllocator<256>      m_slab256;                      ///< The 256 byte slab allocator.
    SlabAllocator<512>      m_slab512;                      ///< The 512 byte slab allocator.
    SlabAllocator<1024>     m_slab1024;                     ///< The 1024 byte slab allocator.
    SlabAllocator<2048>     m_slab2048;                     ///< The 2048 byte slab allocator.
    SlabAllocator<4096>     m_pageDirectoryAllocator;       ///< The 4096 byte slab allocator.

    friend class Vmm;
};

} // namespace MM

} // namespace BartOS

#endif // KERNEL_HEAP_H
