#include "PageTableEntry.h"

#include "PageTable.h"

namespace BartOS
{

namespace x86_64
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

bool PageTableEntry::IsWritable() const
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

void PageTableEntry::SetPageFlags(const PageFlags pageFlags)
{
    SetPresent(pageFlags & PRESENT);
    SetWritable(pageFlags & WRITABLE);
    SetUserAccessible(pageFlags & USER_ACCESSIBLE);
    SetWriteThrough(pageFlags & WRITE_THROUGH);
    SetCacheDisabled(pageFlags & CACHE_DISABLED);
    SetHugePage(pageFlags & HUGE_PAGE);
    SetGlobal(pageFlags & GLOBAL);
    SetNoExecute(pageFlags & NO_EXECUTE);
}

// ---------------------------------------------------------------------------------------------------------

PageFlags PageTableEntry::GetPageFlags() const
{
    PageFlags pageFlags = NO_FLAGS;
    pageFlags |= (IsPresent() ? PRESENT : static_cast<PageFlags>(0));
    pageFlags |= (IsWritable() ? WRITABLE : static_cast<PageFlags>(0));
    pageFlags |= (IsUserAccessible() ? USER_ACCESSIBLE : static_cast<PageFlags>(0));
    pageFlags |= (IsWriteThrough() ? WRITE_THROUGH : static_cast<PageFlags>(0));
    pageFlags |= (IsCacheDisabled() ? CACHE_DISABLED : static_cast<PageFlags>(0));
    pageFlags |= (IsHugePage() ? HUGE_PAGE : static_cast<PageFlags>(0));
    pageFlags |= (IsGlobal() ? GLOBAL : static_cast<PageFlags>(0));
    pageFlags |= (IsNoExecute() ? NO_EXECUTE : static_cast<PageFlags>(0));

    return pageFlags;
}

// ---------------------------------------------------------------------------------------------------------

void PageTableEntry::SetPhysAddr(const BartOS::PhysAddr &physAddr)
{
    Set<PageTableEntry::PhysAddr>(physAddr.Get() >> 12);
}

// ---------------------------------------------------------------------------------------------------------

const BartOS::PhysAddr PageTableEntry::GetPhysAddr() const
{
    return BartOS::PhysAddr(Get<PageTableEntry::PhysAddr>() << 12);
}

} // namespace x86_64

} // namespace BartOS
