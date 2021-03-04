#include "PageTable.h"

namespace BartOS
{

namespace x86_64
{

PageTable::PageTable()
{
}

// ---------------------------------------------------------------------------------------------------------

PageTable::~PageTable()
{
}

// ---------------------------------------------------------------------------------------------------------

template <PageMapLevel LEVEL>
PageTableEntry &PageTable::GetPte(const VirtualAddress &virtualAddress)
{
    const TableEntryIndex entryIndex = PageLevelToAddrLevel<LEVEL>::Level::Get(virtualAddress.Get());
    return m_entries[entryIndex];
}

template PageTableEntry &PageTable::GetPte<PML4>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<PML3>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<PML2>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<PML1>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<PAGE_LEVEL>(const VirtualAddress &virtualAddress);

// ---------------------------------------------------------------------------------------------------------

template <PageMapLevel LEVEL>
const PageTableEntry &PageTable::GetPte(const VirtualAddress &virtualAddress) const
{
    const TableEntryIndex entryIndex = PageLevelToAddrLevel<LEVEL>::Level::Get(virtualAddress.Get());
    return m_entries[entryIndex];
}

template const PageTableEntry &PageTable::GetPte<PML4>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<PML3>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<PML2>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<PML1>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<PAGE_LEVEL>(const VirtualAddress &virtualAddress) const;

} // namespace x86_64

} // namespace BartOS
