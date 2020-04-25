#ifndef VMM_H
#define VMM_H

#include "Kernel/BartOS.h"
#include "Libraries/Misc/Singleton.h"

#include "Paging/PageTable.h"

namespace BartOS
{

namespace MM
{

class Vmm : public Singleton<Vmm>
{
public:
    //! Constructor
    Vmm();

    /*
     *  @brief Map an address 
     * 
     *  @param pP4Table pointer to the Level 4 Page Table.
     *  @param physicalAddress the physical address of the physical page.
     *  @param virtualAddress the virtual address of the virtual page.
     *  @param pageFlags the page flags.
     * 
     *  @retval STATUS_CODE_SUCCESS
     *  @retval STATUS_CODE_ALREADY_MAPPED
     *  @retval ...
     */
    StatusCode MapPage(PageTable * const pP4Table, const PhysicalAddress &physicalAddress, const VirtualAddress &virtualAddress,
        const PageFlags pageFlags, const PageSize pageSize);

    /*
     *  @brief Is an address mapped.
     * 
     *  @param pP4Table pointer to the Level 4 Page Table.
     *  @param virtualAddress the virtual address of the virtual page.
     * 
     *  @return whether address is mapped.
     */
    bool IsAddressMapped(const PageTable * const pP4Table, const VirtualAddress &virtualAddress);

    //! The kernel p4 table.
    static PageTable *m_pKernelP4Table;

private:
    static const uint64_t TEMP_ENTRY_ADDR_BASE = 0xFFFFFFFFFFE00000;

    static PageTable * const m_pTempMapTable;  ///< Level 1 page table used to map temporary pages. Always mapped as last 2MiB in kernel address space.

    /*
     *  @brief Map a temporary page level impl.
     * 
     *  @param physicalAddress the physical address of the physical page.
     * 
     *  @return the mapped page table entry.
     */
    template <PageTableLevel LEVEL>
    static void *MapPageLevelImpl(const PhysicalAddress &physicalAddress);

    /*
     *  @brief Map a temporary page level in the last 2 MiB of the kernel address space.
     * 
     *  @param physicalAddress the physical address of the physical page.
     * 
     *  @return the mapped page table level.
     */
    template <PageTableLevel LEVEL>
    static PageTable *MapPageLevel(const PhysicalAddress &physicalAddress);

    /*
     *  @brief Map a temporary page in the last 2 MiB of the kernel address space.
     * 
     *  @param physicalAddress the physical address of the physical page.
     * 
     *  @return the mapped page.
     */
    static uint8_t *MapPage(const PhysicalAddress &physicalAddress);

    friend class Singleton<Vmm>;
};

} // namespace MM

} // namespace BartOS

#endif // VMM_H
