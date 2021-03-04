#ifndef PML_H
#define PML_H

#include "BartOS.h"

#include "Paging.h"

#include "Kernel/Memory/PhysicalPage.h"
#include "Libraries/Misc/RefPtr.h"

namespace BartOS
{

namespace MM
{

/*
 *  @brief Abstraction class for a Page Table Entry.
 */
class PageTableEntry
{
public:
    /*
     *  @brief Construct the object.
     * 
     *  @param pPte pointer to the pte object.
     */
    PageTableEntry(x86_64::PageTableEntry * const pPte);

    //! Get the Physical Address of the object.
    PhysicalAddress GetPhysicalAddress() const;

    //! Get Page Flags.
    PageFlags GetPageFlags() const;

protected:
    //! Delete copy and move construction and assignment.
    PageTableEntry(const PageTableEntry &rhs) = delete;
    PageTableEntry &operator=(const PageTableEntry &rhs) = delete;
    PageTableEntry(PageTableEntry &&rhs) = delete;
    PageTableEntry &operator=(PageTableEntry &&rhs) = delete;

    x86_64::PageTableEntry  *m_pPte;            ///< Pointer to the pte object.
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Abstraction class for a Page Table.
 */
template <typename ENTRY_TYPE, PageMapLevel PML>
class Pml : public PageTableEntry
{
public:
    using Type = Pml<ENTRY_TYPE, PML>;
    using Entry = ENTRY_TYPE;
    using SubDirectory = Entry;
    using AddressBitField = typename PageLevelToAddrLevel<PML>::Level;

    using Hasher = Hash<uint64_t>;
    using SubDirectoryMap = HashMap<PageTableIndex, SubDirectory *, Hasher>;

    static constexpr PageMapLevel m_level       = PML;
    static constexpr PageSize m_pageSize        = PmlPageSize<m_level>::value;

    static constexpr PageFlags PAGE_DIR_FLAGS   = (WRITABLE | PRESENT);

    /*
     *  @brief Construct the object.
     * 
     *  @param pte page table entry.
     */
    Pml(x86_64::PageTableEntry &pte);

    /*
     *  @brief Construct the object.
     *  Only used for the kernel address space initialization.
     * 
     *  @param pPageTable pointer to the page table.
     */
    Pml(x86_64::PageTable * const pPageTable, kernel_init_tag);

    /*
     *  @brief Construct the object.
     *  Only used for the kernel address space initialization.
     * 
     *  @param pte page table entry.
     */
    Pml(x86_64::PageTableEntry &pte, kernel_init_tag);

    //! Destructor.
    ~Pml();

    /*
     *  @brief Map a virtual page.
     * 
     *  @param physicalAddress the physical address of the physical page.
     *  @param virtualAddress the virtual address of the virtual page.
     *  @param pageFlags the page flags.
     */
    void MapPage(const PhysicalAddress &physicalAddress, const VirtualAddress &virtualAddress,
        const PageFlags pageFlags, const PageSize pageSize);

    /*
     *  @brief Add an entry.
     * 
     *  @param pageTableIndex the page table index.
     *  @param pSubDirectory pointer to the sub directory object.
     */
    void AddEntry(const PageTableIndex pageTableIndex, SubDirectory * const pSubDirectory);

    /*
     *  @brief Iterate over page table.
     *  Recursive callbacks need a terminator statement.
     * 
     *  @param callback callback for each entry.
     *  @param pData pointer to additional data.
     */
    template<typename T>
    void ForEach(T callback, void *pData);

    /*
     *  @brief Iterate over page table.
     *  Recursive callbacks need a terminator statement.
     * 
     *  @param callback callback for each entry.
     *  @param pData pointer to additional data.
     */
    template<typename T>
    void ForEach(T callback, void *pData) const;

private:
    using Base = PageTableEntry;

    x86_64::PageTable               *m_pPageTable;              ///< Pointer to the page table.
    bool                            m_ownsPageTable;            ///< Whether the object owns the page table.

    Hasher                          m_hasher;                   ///< Hasher object for the map.
    SubDirectoryMap                 m_subDirectoryMap;          ///< The subdirectory map.
    
    friend class AddressSpace;
};

// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
template<typename T>
inline auto Pml<ENTRY_TYPE, PML>::ForEach(T callback, void *pData) -> void
{
    typename SubDirectoryMap::iterator it = m_subDirectoryMap.begin();
    for (; it != m_subDirectoryMap.end(); ++it)
    {
        const PageTableIndex entryIndex = it->template get<0>();
        Entry * const pEntry = it->template get<1>();
        callback(entryIndex, *pEntry, pData);
    }
}

// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
template<typename T>
inline auto Pml<ENTRY_TYPE, PML>::ForEach(T callback, void *pData) const -> void
{
    typename SubDirectoryMap::const_iterator it = m_subDirectoryMap.cbegin();
    for (; it != m_subDirectoryMap.cend(); ++it)
    {
        const PageTableIndex entryIndex = it->template get<0>();
        const Entry * const pEntry = it->template get<1>();
        callback(entryIndex, *pEntry, pData);
    }
}

// ---------------------------------------------------------------------------------------------------------

using PageEntry = PageTableEntry;
using Pml1      = Pml<PageEntry, PML1>;
using Pml2      = Pml<Pml1, PML2>;
using Pml3      = Pml<Pml2, PML3>;
using Pml4      = Pml<Pml3, PML4>;

} // namespace MM

} // namespace BartOS

#endif // PAGE_TABLE_LEVEL_H
