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
    static const size_t MAX_KERNEL_PAGES = MAX_KERNEL_SIZE / PAGE_SIZE_2M;

    /*
     *  @brief Constructor
     *
     *  Can be used before the memory manager has been initialized.
     *
     *  @param  pPageTable the pointer to the page table.
     *
     */
    KernelAddressSpace(MM::PageTable * const pPageTable);

    //! Initialize the address space.
    void Initialize();

    //! Synchronize the kernel address space.
    void SynchronizeKernelAddressSpace();

    VirtualPage     *m_pVirtualPages;   ///< The virtual page pointer pool.
    size_t          m_nVirtualPages;    ///< The number of virtual pages.

    friend class MM::Vmm;
};

} // namespace MM

} // namespace BartOS

#endif // KERNEL_ADDRESS_SPACE_H
