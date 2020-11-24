#include "PageTableEntry.h"

#include "PageTable.h"

namespace BartOS
{

namespace MM
{

bool PageTableEntry::SetPresent(const bool isPresent)
{
    Set<Present>(isPresent);

    return isPresent;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::SetWritable(const bool isWritable)
{
    Set<Writable>(isWritable);

    return isWritable;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::SetUserAccessible(const bool isUserAccessible)
{
    Set<UserAccessible>(isUserAccessible);

    return isUserAccessible;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::SetWriteThrough(const bool isWriteThrough)
{
    Set<WriteThrough>(isWriteThrough);

    return isWriteThrough;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::SetCacheDisabled(const bool isCacheDisabled)
{
    Set<CacheDisabled>(isCacheDisabled);

    return isCacheDisabled;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::SetHugePage(const bool isHugePage)
{
    Set<HugePage>(isHugePage);

    return isHugePage;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::SetGlobal(const bool isGlobal)
{
    Set<Global>(isGlobal);

    return isGlobal;
}

// ---------------------------------------------------------------------------------------------------------

PageTableEntry::Available::ValueType PageTableEntry::SetAvailable1(const Available::ValueType available1)
{
    Set<Available>(available1);

    return available1;
}

// ---------------------------------------------------------------------------------------------------------

PageTableEntry::Available2::ValueType PageTableEntry::SetAvailable2(const Available2::ValueType available2)
{
    Set<Available2>(available2);
    
    return available2;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::SetNoExecute(const bool isNoExecute)
{
    Set<NoExecute>(isNoExecute);

    return isNoExecute;
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsPresent() const
{
    return Get<Present>();
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsWritablePresent() const
{
    return Get<Writable>();
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsUserAccessible() const
{
    return Get<UserAccessible>();
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsWriteThrough() const
{
    return Get<WriteThrough>();
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsCacheDisabled() const
{
    return Get<CacheDisabled>();
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsHugePage() const
{
    return Get<HugePage>();
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsGlobal() const
{
    return Get<Global>();
}

// ---------------------------------------------------------------------------------------------------------

PageTableEntry::Available::ValueType PageTableEntry::GetAvailable1() const
{
    return Get<Available>();
}

// ---------------------------------------------------------------------------------------------------------

PageTableEntry::Available2::ValueType PageTableEntry::GetAvailable2() const
{
    return Get<Available2>();
}

// ---------------------------------------------------------------------------------------------------------

bool PageTableEntry::IsNoExecute() const
{
    return Get<NoExecute>();
}

// ---------------------------------------------------------------------------------------------------------

void PageTableEntry::SetPhysicalAddress(const BartOS::PhysicalAddress &physicalAddress)
{
    Set<PageTableEntry::PhysicalAddress>(physicalAddress.Get() >> 12);
}

// ---------------------------------------------------------------------------------------------------------

const BartOS::PhysicalAddress PageTableEntry::GetPhysicalAddress() const
{
    return BartOS::PhysicalAddress(Get<PageTableEntry::PhysicalAddress>() << 12);
}

// ---------------------------------------------------------------------------------------------------------

PageTable *PageTableEntry::PageTablePtr()
{
    const Address_t address = (Get<PhysicalAddress>() << 12);
    return reinterpret_cast<PageTable *>(ALIGN(address, PAGE_SIZE));
}

// ---------------------------------------------------------------------------------------------------------

const PageTable *PageTableEntry::PageTablePtr() const
{
    const Address_t address = (Get<PhysicalAddress>() << 12);
    return reinterpret_cast<PageTable *>(ALIGN(address, PAGE_SIZE));
}

// ---------------------------------------------------------------------------------------------------------

uint8_t *PageTableEntry::PagePtr()
{
    const Address_t address = (Get<PhysicalAddress>() << 12);
    return reinterpret_cast<uint8_t *>(ALIGN(address, PAGE_SIZE));
}

// ---------------------------------------------------------------------------------------------------------

const uint8_t *PageTableEntry::PagePtr() const
{
    const Address_t address = (Get<PhysicalAddress>() << 12);
    return reinterpret_cast<uint8_t *>(ALIGN(address, PAGE_SIZE));
}

} // namespace MM

} // namespace BartOS
