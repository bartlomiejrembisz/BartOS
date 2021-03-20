#ifndef KERNEL_ADDRESS_SPACE_H
#define KERNEL_ADDRESS_SPACE_H

//#include "Paging/Pml.h"

#include "VmArea.h"

namespace BartOS
{

namespace MM
{

/*
 *  @brief The address space class.
 */
class AddressSpace
{
public:
    //! Destructor.
    ~AddressSpace();

    /*
     *  @brief Allocate memory for this address space.
     * 
     *  @param  nBytes ammount to allocate.
     *  @param  pageSize the page size.
     *  @param  pageFlags the page flags
     */
    void *Allocate(size_t nBytes, const PageSize pageSize, const PageFlags pageFlags = NO_FLAGS);

    /*
     *  @brief Allocate memory for this address space.
     * 
     *  @param  pVmArea pointer to the Vm Area.
     */
    void AddVmArea(VmArea *pVmArea);

    /*
     *  @brief Create the kernel address space.
     * 
     *  @param pPageTable the page table.
     * 
     *  @return the kernel address space.
     */
    static AddressSpace *CreateKernelAddressSpace(x86_64::PageTable * const pPageTable);

private:
    /*
     *  @brief Constructor
     *
     *  Used to initialize the kernel address space.
     *
     *  @param  pPageTable the pointer to the page table.
     */
    AddressSpace(x86_64::PageTable * const pPageTable, kernel_init_tag);

    /*
     *  @brief Synchronize the page tables.
     */
    void SynchronizePml();

    //MM::Pml4                    m_pml4;                 ///< Pointer to the Pml object.

    VmArea                      m_textVmArea;           ///< The text vm area.
    VmArea                      m_roDataVmArea;         ///< The ro data vm area.
    VmArea                      m_dataVmArea;           ///< The data vm area.
    VmArea                      m_bssVmArea;            ///< The bss vm area.
    VmArea                      m_heapVmArea;           ///< The heap vm area.
    VmArea                      m_stackVmArea;          ///< The stack vm area.

    //VmAreaList                  m_vmAreaList;           ///< The VmArea lookup table.

    friend class MM::Vmm;
};

// ---------------------------------------------------------------------------------------------------------

inline AddressSpace::~AddressSpace()
{
}

} // namespace MM

} // namespace BartOS

#endif // KERNEL_ADDRESS_SPACE_H
