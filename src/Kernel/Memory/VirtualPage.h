#ifndef VIRTUAL_PAGE_H
#define VIRTUAL_PAGE_H

#include "Kernel/BartOS.h"
#include "Kernel/Memory/Paging/PageTableEntry.h"

#include "MemoryPool.h"

namespace BartOS
{

namespace MM
{

// Forward declaration.
class AddressSpace;
class KernelAddressSpace;
class PhysicalPage;

/*
 *  @brief The Virtual Page class.
 */
class VirtualPage
{
public:
    /*
     *  @brief Get the virtual address of the page
     * 
     *  @return the virtual address of the page.
     */
    VirtualAddress GetAddress() const;

    /*
     *  @brief Is the page initialized.
     * 
     *  @return whether the page is initialized.
     */
    bool IsInitialized() const;

private:
    //! Constructor
    VirtualPage();

    //! Disable default and copy construction.
    VirtualPage(const VirtualPage &rhs) = delete;
    VirtualPage &operator=(const VirtualPage &rhs) = delete;

    /*
     *  @brief Constructor
     *  Only the AddressSpace can create a virtual page.
     * 
     *  @param vaddr the virtual address.
     *  @param addressSpace the address space owning this page.
     *  @param pPte pointer to the page table entry.
     *  @param physicalRange the physical range.
     *  @param pageSize the size of the page.
     */
    VirtualPage(const VirtualAddress vaddr, AddressSpace &addressSpace, MM::PageTableEntry * const pPte,
        const MemoryPool::PhysicalRange &physicalRange, const PageSize pageSize);

    /*
     *  @brief Initialize the object.
     *  Only the AddressSpace can create a virtual page.
     * 
     *  @param vaddr the virtual address.
     *  @param addressSpace the address space owning this page.
     *  @param pPte pointer to the page table entry.
     *  @param pageSize the size of the page.
     */
    void Initialize(const VirtualAddress vaddr, AddressSpace &addressSpace, MM::PageTableEntry * const pPte,
        const MemoryPool::PhysicalRange &physicalRange, const PageSize pageSize);

    VirtualAddress              m_addr;             ///< The virtual address of the page.
    AddressSpace                *m_pAddressSpace;   ///< The Address Space pointer.
    MM::PageTableEntry          *m_pPte;            ///< The Page Table Entry pointer.
    MemoryPool::PhysicalRange   m_physicalRange;    ///< The physical range for the page.
    PageSize                    m_pageSize;         ///< The size of the page

    friend class AddressSpace;
    friend class KernelAddressSpace;
};

// ---------------------------------------------------------------------------------------------------------

inline bool VirtualPage::IsInitialized() const
{
    return (m_pAddressSpace && m_pPte);
}

} // name MM

} // namespace BartOS

#endif // VIRTUAL_PAGE_H
