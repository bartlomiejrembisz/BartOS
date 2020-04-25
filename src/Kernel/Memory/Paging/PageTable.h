#ifndef PAGE_TABLE_LEVEL_H
#define PAGE_TABLE_LEVEL_H

#include "Kernel/BartOS.h"
#include "PageTableEntry.h"

namespace BartOS
{

namespace MM
{

/*
 *  @brief Class representing a page table level.
 */
class PageTable
{
public:
    static constexpr TableEntryIndex PAGE_TABLE_COUNT = 512;
    static constexpr PageTableIndex PAGE_TABLE_LEVEL_COUNT = 4;

    //! Constructor.
    PageTable();

    //! Destructor.
    ~PageTable();

    /*
     *  @brief Get the page table entry
     * 
     *  @param virtualAddress the virtual address.
     * 
     *  @tparam LEVEL the page table level enum.
     * 
     *  @return the page table level entry.
     */
    template <PageTableLevel LEVEL>
    PageTableEntry &GetPte(const VirtualAddress &virtualAddress);

    /*
     *  @brief Get the page table entry
     * 
     *  @param virtualAddress the virtual address.
     * 
     *  @return the page table level entry.
     */
    template <PageTableLevel LEVEL>
    const PageTableEntry &GetPte(const VirtualAddress &virtualAddress) const;

    PageTableEntry  m_entries[PAGE_TABLE_COUNT];
};

static_assert(sizeof(PageTable) == 4096);

} // namespace MM

} // namespace BartOS

#endif // PAGE_TABLE_LEVEL_H
