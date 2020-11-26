#ifndef KERNEL_ADDRESS_SPACE_H
#define KERNEL_ADDRESS_SPACE_H

#include "AddressSpace.h"

namespace BartOS
{

namespace MM
{

/*
 *  @brief The kernel address space class.
 */
class KernelAddressSpace final : public AddressSpace
{
public:
    //! AddressSpace interface.
    virtual void *Allocate(size_t nBytes, const PageSize pageSize, const PageFlags pageFlags = NO_FLAGS) override;

    /*
     *  @brief Is the virtual address a higher half (kernel) address.
     *
     *  @param  vAddr the virtual address.
     * 
     *  @return whether the address is a kernel address.
     */
    static bool IsKernelAddress(const VirtualAddress vAddr);

private:
    static const Address_t TEMP_MAP_ADDR_BASE = TEMP_MAP_ADDR;                                      ///< The address of the temporary mapping.
    static const size_t TEMP_MAP_SIZE = (0xFFFFFFFFFFFFFFFF - TEMP_MAP_ADDR_BASE);                  ///< The size of the temp map.
    
    static const size_t MAX_SMALL_KERNEL_PAGES = (MAX_KERNEL_SIZE - (TEMP_MAP_SIZE)) / PAGE_2M;     ///< Maximum amount of small kernel pages.
    static const size_t MAX_HUGE_KERNEL_PAGES = (MAX_KERNEL_SIZE - (TEMP_MAP_SIZE)) / PAGE_4K;      ///< Minimum amount of huge pages.

    static const VirtualAddress KERNEL_BEGIN_ADDRESS;           ///< The address of the start of the kernel.

    /*
     *  @brief Constructor
     *
     *  Can be used before the memory manager has been initialized.
     *
     *  @param  pPageTable the pointer to the page table.
     */
    KernelAddressSpace(MM::PageTable * const pPageTable);

    //! Initialize the address space.
    void Initialize();

    //! Synchronize the kernel address space.
    void SynchronizeKernelAddressSpace();

    /*
     *  @brief Find the huge page table entry from a virtual address.
     *
     *  @param vAddr the virtual address.
     * 
     *  @return pointer to the PTE
     */
    MM::PageTableEntry *FindPageTableEntry(const VirtualAddress vAddr);

    /*
     *  @brief Get the maximum kernel address space size.
     *
     *  @return the maximum size of the kernel address space.
     */
    size_t GetMaximumKernelAddressSpaceSize();

    VMArea          m_kernelVMArea;     ///< The contiguous kernel VMArea.
    VMArea          **m_pVMAreaPtr;     ///< The VMArea lookup table.
    size_t          m_nVMAreas;         ///< The number of small pages allocated.

    friend class MM::Vmm;
};

// ---------------------------------------------------------------------------------------------------------

inline size_t KernelAddressSpace::GetMaximumKernelAddressSpaceSize()
{
    return MAX_KERNEL_SIZE;
}

} // namespace MM

} // namespace BartOS

#endif // KERNEL_ADDRESS_SPACE_H
