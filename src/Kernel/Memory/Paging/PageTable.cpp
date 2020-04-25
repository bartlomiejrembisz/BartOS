#include "PageTable.h"

namespace BartOS
{

namespace MM
{

PageTable::PageTable()
{
}

// ---------------------------------------------------------------------------------------------------------

PageTable::~PageTable()
{
}

// ---------------------------------------------------------------------------------------------------------

template <PageTableLevel LEVEL>
PageTableEntry &PageTable::GetPte(const VirtualAddress &virtualAddress)
{
    const TableEntryIndex entryIndex = PageLevelToAddrLevel<LEVEL>::Level::Get(virtualAddress.Get());
    return m_entries[entryIndex];
}

template PageTableEntry &PageTable::GetPte<TABLE_LEVEL4>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<TABLE_LEVEL3>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<TABLE_LEVEL2>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<TABLE_LEVEL1>(const VirtualAddress &virtualAddress);
template PageTableEntry &PageTable::GetPte<PAGE_LEVEL>(const VirtualAddress &virtualAddress);

// ---------------------------------------------------------------------------------------------------------

template <PageTableLevel LEVEL>
const PageTableEntry &PageTable::GetPte(const VirtualAddress &virtualAddress) const
{
    const TableEntryIndex entryIndex = PageLevelToAddrLevel<LEVEL>::Level::Get(virtualAddress.Get());
    return m_entries[entryIndex];
}

template const PageTableEntry &PageTable::GetPte<TABLE_LEVEL4>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<TABLE_LEVEL3>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<TABLE_LEVEL2>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<TABLE_LEVEL1>(const VirtualAddress &virtualAddress) const;
template const PageTableEntry &PageTable::GetPte<PAGE_LEVEL>(const VirtualAddress &virtualAddress) const;

} // namespace MM

} // namespace BartOS
