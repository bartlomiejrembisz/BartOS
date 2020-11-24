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
class KernelAddressSpace : public AddressSpace
{
public:
    /*
     *  @brief Retrieve a Virtual Page object.
     * 
     *  @param virtualAddress the virtual address of the page.
     */
    virtual VirtualPage *GetVirtualPage(const VirtualAddress virtualAddress) override;

private:
    static const size_t MINIMUM_KERNEL_PAGES = MIN_KERNEL_SIZE / PAGE_2M;       ///< Minimum amount of kernel pages.
    static const size_t KERNEL_MEMORY_FACTOR = 6;                               ///< What factor of total memory should be given to the kernel

    static const PageSize KERNEL_PAGE_SIZE = PAGE_2M;                           ///< The kernel page size 

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
     *  @brief Get the maxium kernel address space size.
     *
     *  @return the maximum size of the kernel address space.
     */
    size_t GetMaximumKernelAddressSpaceSize();

    VirtualPage     *m_pVirtualPages;   ///< The virtual page pointer pool.
    size_t          m_nVirtualPages;    ///< The number of virtual pages.

    friend class MM::Vmm;
};

// ---------------------------------------------------------------------------------------------------------

inline size_t KernelAddressSpace::GetMaximumKernelAddressSpaceSize()
{
    if ((!m_pVirtualPages) && (m_nVirtualPages == 0))
        return ~0;

    return m_nVirtualPages * KERNEL_PAGE_SIZE;
}

} // namespace MM

} // namespace BartOS

#endif // KERNEL_ADDRESS_SPACE_H
