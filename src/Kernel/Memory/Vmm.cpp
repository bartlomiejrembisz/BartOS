#include "Vmm.h"
#include "Kernel/Arch/x86_64/CPU.h"

//! Initial P4 table p4_table symbol exposed from boot.asm
extern "C" BartOS::MM::PageTable p4_table;

//! A P1 level table 
extern "C" BartOS::MM::PageTable p1_temp_map_table;

namespace BartOS
{

namespace MM
{

PageTable *Vmm::m_pKernelP4Table = &p4_table;

PageTable * const Vmm::m_pTempMapTable = &p1_temp_map_table;

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

Vmm::Vmm()
{
}

// ---------------------------------------------------------------------------------------------------------

void Vmm::Initialize()
{
    
}

// ---------------------------------------------------------------------------------------------------------

StatusCode Vmm::MapPage(PageTable * const pP4Table, const PhysicalAddress &physicalAddress, const VirtualAddress &virtualAddress,
        const PageFlags pageFlags, const PageSize pageSize)
{
    const StatusCode statusCode = MapPageImpl(pP4Table, physicalAddress, virtualAddress, pageFlags, pageSize);
    if (STATUS_CODE_SUCCESS == statusCode)
        CPU::Invlpg(virtualAddress);

    return statusCode;
}

// ---------------------------------------------------------------------------------------------------------

bool Vmm::IsAddressMapped(const PageTable * const pP4Table, const VirtualAddress &virtualAddress)
{
    if (TEMP_ENTRY_ADDR_BASE <= virtualAddress.Get())
        return true;

    //! Get PTE of the p3 table and check if it exists. If not return false;
    const PageTableEntry &p4TableEntry = pP4Table->GetPte<TABLE_LEVEL4>(virtualAddress);
    const PageTable *pP3Table = nullptr;
    if (!p4TableEntry.IsPresent())
        return false;

    pP3Table = MapPageLevel<TABLE_LEVEL3>(p4TableEntry.GetPhysicalAddress());

    //! Get PTE of the p2 table and check if it exists. If not return false;
    const PageTableEntry &p3TableEntry = pP3Table->GetPte<TABLE_LEVEL3>(virtualAddress);
    const PageTable *pP2Table = nullptr;
    if (!p3TableEntry.IsPresent())
        return false;
    else if (p3TableEntry.IsHugePage())
        return true;

    pP2Table = MapPageLevel<TABLE_LEVEL2>(p3TableEntry.GetPhysicalAddress());

    //! Get PTE of the p1 table and check if it exists. If not return false;
    const PageTableEntry &p2TableEntry = pP2Table->GetPte<TABLE_LEVEL2>(virtualAddress);
    const PageTable *pP1Table = nullptr;
    if (!p2TableEntry.IsPresent())
        return false;
    else if (p2TableEntry.IsHugePage())
        return true;

    pP1Table = MapPageLevel<TABLE_LEVEL1>(p2TableEntry.GetPhysicalAddress());

    //! Get page and check if it exists. If not return false;
    const PageTableEntry &p1TableEntry = pP1Table->GetPte<TABLE_LEVEL1>(virtualAddress);
    if (!p1TableEntry.IsPresent())
        return false;

    return true;
}

// ---------------------------------------------------------------------------------------------------------

StatusCode Vmm::MapPageImpl(PageTable * const pP4Table, const PhysicalAddress &physicalAddress, const VirtualAddress &virtualAddress,
        const PageFlags pageFlags, const PageSize pageSize)
{
    ASSERT(pP4Table);
    if (TEMP_ENTRY_ADDR_BASE <= virtualAddress.Get())
        return STATUS_CODE_RESERVED;
    
    //! Get PTE of the p3 table and check if it's present. Allocate a physical page for it if it doesn't exist.
    PageTableEntry &p4TableEntry = pP4Table->GetPte<TABLE_LEVEL4>(virtualAddress);
    PageTable *pP3Table = nullptr;
    if (!p4TableEntry.IsPresent())
    {
        // TODO: Allocate a physical page.
    }
    else
    {
        pP3Table = MapPageLevel<TABLE_LEVEL3>(p4TableEntry.GetPhysicalAddress());
    }

    //! Get PTE of the p3 table and check if it's present. Allocate a physical page for it if it doesn't exist.
    PageTableEntry &p3TableEntry = pP3Table->GetPte<TABLE_LEVEL3>(virtualAddress);
    PageTable *pP2Table = nullptr;
    if (!p3TableEntry.IsPresent())
    {
        // TODO: Allocate a physical page.
    }
    else
    {
        pP2Table = MapPageLevel<TABLE_LEVEL2>(p3TableEntry.GetPhysicalAddress());
    }    

    //! Get PTE of the p2 table and check if it's present. Allocate a physical page for it if it doesn't exist.
    PageTableEntry &p2TableEntry = pP2Table->GetPte<TABLE_LEVEL2>(virtualAddress);
    PageTable *pP1Table = nullptr;
    if (!p2TableEntry.IsPresent())
    {
        // TODO: Allocate a physical page.
    }
    else
    {
        pP1Table = MapPageLevel<TABLE_LEVEL1>(p2TableEntry.GetPhysicalAddress());
    }

    if ((pageFlags & PageFlags::HUGE_PAGE) && (PAGE_2M == pageSize))
    {
        ASSERT(ALIGN(physicalAddress.Get(), PAGE_SIZE_2M) == physicalAddress.Get());
        //! 52 bit page size aligned address.
        p2TableEntry.SetPhysicalAddress(physicalAddress);

        //! Set the entry flags.
        p2TableEntry.SetPresent(pageFlags & PRESENT);
        p2TableEntry.SetWritable(pageFlags & WRITABLE);
        p2TableEntry.SetUserAccessible(pageFlags & USER_ACCESSIBLE);
        p2TableEntry.SetWriteThrough(pageFlags & WRITE_THROUGH);
        p2TableEntry.SetCacheDisabled(pageFlags & DISABLE_CACHE);
        p2TableEntry.SetHugePage(pageFlags & HUGE_PAGE);
        p2TableEntry.SetGlobal(pageFlags & GLOBAL);
        p2TableEntry.SetNoExecute(pageFlags & NO_EXECUTE);

        return STATUS_CODE_SUCCESS;
    }
    else
    {
        // Can't be huge page and not a 2M page at this point.
        ASSERT(false);
    }

    //! Get PTE of the p1 table and check if it exists. Allocate a physical page for it if it doesn't exist.
    PageTableEntry &p1TableEntry = pP1Table->GetPte<TABLE_LEVEL1>(virtualAddress);

    ASSERT(ALIGN(physicalAddress.Get(), PAGE_SIZE) == physicalAddress.Get());
    //! 52 bit page size aligned address.
    p1TableEntry.SetPhysicalAddress(physicalAddress);

    //! Set the entry flags.
    p1TableEntry.SetPresent(pageFlags & PRESENT);
    p1TableEntry.SetWritable(pageFlags & WRITABLE);
    p1TableEntry.SetUserAccessible(pageFlags & USER_ACCESSIBLE);
    p1TableEntry.SetWriteThrough(pageFlags & WRITE_THROUGH);
    p1TableEntry.SetCacheDisabled(pageFlags & DISABLE_CACHE);
    p1TableEntry.SetHugePage(0);    //< Huge pages ignored at this point.
    p1TableEntry.SetGlobal(pageFlags & GLOBAL);
    p1TableEntry.SetNoExecute(pageFlags & NO_EXECUTE);

    return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------

template <PageTableLevel LEVEL>
void *Vmm::MapPageLevelImpl(const PhysicalAddress &physicalAddress)
{
    VirtualAddress virtualAddress(TEMP_ENTRY_ADDR_BASE);
    virtualAddress.SetLevel1(virtualAddress.GetLevel1() + static_cast<uint8_t>(LEVEL));

    PageTableEntry &entry = m_pTempMapTable->m_entries[LEVEL];
    entry.SetPhysicalAddress(physicalAddress);
    entry.SetPresent(1);
    entry.SetWritable(1);
    entry.SetUserAccessible(0);
    entry.SetWriteThrough(1);
    entry.SetCacheDisabled(0);
    entry.SetHugePage(0);
    entry.SetGlobal(0);
    entry.SetNoExecute(0);

    return static_cast<void *>(virtualAddress);
}

// ---------------------------------------------------------------------------------------------------------

template <PageTableLevel LEVEL>
PageTable *Vmm::MapPageLevel(const PhysicalAddress &physicalAddress)
{
    return reinterpret_cast<PageTable *>(MapPageLevelImpl<LEVEL>(physicalAddress));
}

// ---------------------------------------------------------------------------------------------------------

uint8_t *Vmm::MapPage(const PhysicalAddress &physicalAddress)
{
    return reinterpret_cast<uint8_t *>(MapPageLevelImpl<PAGE_LEVEL>(physicalAddress));
}

} // namespace MM

} // namespace BartOS