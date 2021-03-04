#ifndef PAGE_TABLE_LEVEL_H
#define PAGE_TABLE_LEVEL_H

#include "BartOS.h"
#include "PageTableEntry.h"

namespace BartOS
{

namespace x86_64
{

/*
 *  @brief Class representing a page table level.
 */
class PageTable
{
public:
    static constexpr TableEntryIndex PAGE_TABLE_COUNT = 512;

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
    template <PageMapLevel LEVEL>
    PageTableEntry &GetPte(const VirtualAddress &virtualAddress);

    /*
     *  @brief Get the page table entry
     * 
     *  @param virtualAddress the virtual address.
     * 
     *  @return the page table level entry.
     */
    template <PageMapLevel LEVEL>
    const PageTableEntry &GetPte(const VirtualAddress &virtualAddress) const;

    /*
     *  @brief Iterate over page table.
     *  Recursive callbacks need a terminator statement.
     * 
     *  @param callback callback for each entry.
     *  @param pData pointer to additional data.
     */
    template<typename T>
    void ForEachEntry(T callback, void *pData);

    /*
     *  @brief Iterate over page table.
     *  Recursive callbacks need a terminator statement.
     * 
     *  @param callback callback for each entry.
     *  @param pData pointer to additional data.
     */
    template<typename T>
    void ForEachEntry(T callback, void *pData) const;

    PageTableEntry  m_entries[PAGE_TABLE_COUNT];
};

static_assert(sizeof(PageTable) == 4096);

// ---------------------------------------------------------------------------------------------------------

template<typename T>
inline auto PageTable::ForEachEntry(T callback, void *pData) -> void
{
    for (size_t i = 0; i < PAGE_TABLE_COUNT; ++i)
        callback(i, m_entries[i], pData);
}

// ---------------------------------------------------------------------------------------------------------

template<typename T>
inline auto PageTable::ForEachEntry(T callback, void *pData) const -> void
{
    for (size_t i = 0; i < PAGE_TABLE_COUNT; ++i)
        callback(i, m_entries[i], pData);
}

} // namespace x86_64

} // namespace BartOS

#endif // PAGE_TABLE_LEVEL_H
