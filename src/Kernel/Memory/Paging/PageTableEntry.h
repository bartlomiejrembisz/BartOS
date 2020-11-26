#ifndef PAGE_TABLE_ENTRY_H
#define PAGE_TABLE_ENTRY_H

#include "Kernel/BartOS.h"
#include "Libraries/Misc/BitFields.h"

namespace BartOS
{

namespace MM
{

//! Forward declare the page table.
class PageTable;

//! The page table entry class
class PageTableEntry : public Bitmap<64>
{
public:
    typedef BitField<PageTableEntry, 1>     Present;            ///< Is page present.
    typedef BitField<Present, 1>            Writable;           ///< Is page writable.
    typedef BitField<Writable, 1>           UserAccessible;     ///< If not set only kernel can access.
    typedef BitField<UserAccessible, 1>     WriteThrough;       ///< Writes through the cache i.e data both in cache and memory (no dirty data).
    typedef BitField<WriteThrough, 1>       CacheDisabled;      ///< Disable caching the page.
    typedef BitField<CacheDisabled, 1>      Accessed;           ///< Set by the cpu, ignore.
    typedef BitField<Accessed, 1>           Dirty;              ///< Set by the cpu, ignore.
    typedef BitField<Dirty, 1>              HugePage;           ///< Must be 0 in P1 and P4, creates a 1GiB page in P3, creates a 2MiB page in P2.
    typedef BitField<HugePage, 1>           Global;             ///< Address space switch doesn't flush this page from the TLB. PGE in CR4 must be set.
    typedef BitField<Global, 3>             Available;          ///< Can be used freely.
    typedef BitField<Available, 40>         PhysicalAddress;    ///< 52 bit physical address (page aligned, 4K, 2M or 1G)
    typedef BitField<PhysicalAddress, 11>   Available2;         ///< Can be used freely.
    typedef BitField<Available2, 1>         NoExecute;          ///< Forbid executing code on this page (NXE bit in the EFER register must be set).

    //! Setters
    bool SetPresent(const bool isPresent);
    bool SetWritable(const bool isWritable);
    bool SetUserAccessible(const bool isUserAccessible);
    bool SetWriteThrough(const bool isWriteThrough);
    bool SetCacheDisabled(const bool isCacheDisabled);
    bool SetHugePage(const bool isHugePage);
    bool SetGlobal(const bool isGlobal);
    Available::ValueType SetAvailable1(const Available::ValueType available1);
    Available2::ValueType SetAvailable2(const Available2::ValueType available2);
    bool SetNoExecute(const bool isNoExecute);

    //! Getters
    bool IsPresent() const;
    bool IsWritablePresent() const;
    bool IsUserAccessible() const;
    bool IsWriteThrough() const;
    bool IsCacheDisabled() const;
    bool IsHugePage() const;
    bool IsGlobal() const;
    Available::ValueType GetAvailable1() const;
    Available2::ValueType GetAvailable2() const;
    bool IsNoExecute() const;

    /*
     *  @brief Set the page flags.
     * 
     *  @param pageFlags the page flags.
     */
    void SetPageFlags(const PageFlags pageFlags);

    /*
     *  @brief Set the physical address in the entry
     *  Shift the address by 12 bits to the right.
     * 
     *  @param physicalAddress the physical address.
     */
    void SetPhysicalAddress(const BartOS::PhysicalAddress &physicalAddress);

    /*
     *  @brief Set the physical address in the entry
     * 
     *  @return the physical address..
     */
    const BartOS::PhysicalAddress GetPhysicalAddress() const;

    /*
     *  @brief Convert entry to a page table level pointer.
     * 
     *  @return pointer to the underlying page table entry.
     */
    PageTable *PageTablePtr();

    /*
     *  @brief Convert entry to a page table level pointer.
     * 
     *  @return pointer to the underlying page.
     */
    const PageTable *PageTablePtr() const;

    /*
     *  @brief Convert entry to a page pointer.
     * 
     *  @return pointer to the underlying page table entry.
     */
    uint8_t *PagePtr();

    /*
     *  @brief Convert entry to a page pointer.
     * 
     *  @return pointer to the underlying page.
     */
    const uint8_t *PagePtr() const;
};

static_assert(sizeof(PageTableEntry) == 8);

} // namespace MM

} // namespace BartOS

#endif // PAGE_TABLE_ENTRY_H
