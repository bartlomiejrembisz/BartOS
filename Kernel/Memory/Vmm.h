#ifndef VMM_H
#define VMM_H

#include "Arch/x86_64/Interrupts/PageFaultHandler.h"

#include "BartOS.h"
#include "Lib/Core/Singleton.h"

#include "Paging/Paging.h"

#include "AddressSpace.h"
#include "HeapManager.h"

namespace BartOS
{

namespace MM
{

class Vmm : public Singleton<Vmm>
{
public:
    static const Address_t TEMP_MAP_ADDR_BASE = TEMP_MAP_ADDR;                                      ///< The address of the temporary mapping.
    static const size_t TEMP_MAP_SIZE = (0xFFFFFFFFFFFFFFFF - TEMP_MAP_ADDR_BASE);                  ///< The size of the temp map.

    //! Constructor
    Vmm();

    //! Initialize the VMM.
    void Initialize();

    //! Get the kernel heap.
    HeapManager &GetHeapManager();

    /*
     *  @brief Ensure a kernel virtual page is mapped during initialization.
     *
     *  @param physAddr the physical address of the physical page.
     *  @param virtAddr the virtual address of the virtual page.
     */
    void KernelInitMap(const PhysAddr &physAddr, const VirtAddr &virtAddr);

private:
    static x86_64::PageTable * const m_pTempMapTable;  ///< Level 1 page table used to map temporary pages. Always mapped as last 2MiB in kernel address space.

    /*
     *  @brief Map a temporary page level impl.
     * 
     *  @param physAddr the physical address of the physical page.
     *  @param pml the pml.
     * 
     *  @return the mapped page table entry.
     */
    static void *TempMap(const PhysAddr &physAddr, const PageMapLevel pml);

    HeapManager                      m_HeapManager;               ///< The kernel heap.
    AddressSpace                    *m_pKernelAddressSpace;     ///< The kernel address space object.
    x86_64::PageFaultHandler        m_pageFaultHandler;         ///< The page fault handler.
    bool                            m_isInitialized;            ///< Whether the object is initialized.

    friend class PageFaultHandler;
    friend class Singleton<Vmm>;
};

} // namespace MM

} // namespace BartOS

#endif // VMM_H
