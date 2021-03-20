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
PageTableEntry &PageTable::GetPte(const VirtAddr &virtAddr)
{
    const TableEntryIndex entryIndex = PageLevelToAddrLevel<LEVEL>::Level::Get(virtAddr.Get());
    return m_entries[entryIndex];
}

template PageTableEntry &PageTable::GetPte<PML4>(const VirtAddr &virtAddr);
template PageTableEntry &PageTable::GetPte<PML3>(const VirtAddr &virtAddr);
template PageTableEntry &PageTable::GetPte<PML2>(const VirtAddr &virtAddr);
template PageTableEntry &PageTable::GetPte<PML1>(const VirtAddr &virtAddr);
template PageTableEntry &PageTable::GetPte<PAGE_LEVEL>(const VirtAddr &virtAddr);

// ---------------------------------------------------------------------------------------------------------

template <PageMapLevel LEVEL>
const PageTableEntry &PageTable::GetPte(const VirtAddr &virtAddr) const
{
    const TableEntryIndex entryIndex = PageLevelToAddrLevel<LEVEL>::Level::Get(virtAddr.Get());
    return m_entries[entryIndex];
}

template const PageTableEntry &PageTable::GetPte<PML4>(const VirtAddr &virtAddr) const;
template const PageTableEntry &PageTable::GetPte<PML3>(const VirtAddr &virtAddr) const;
template const PageTableEntry &PageTable::GetPte<PML2>(const VirtAddr &virtAddr) const;
template const PageTableEntry &PageTable::GetPte<PML1>(const VirtAddr &virtAddr) const;
template const PageTableEntry &PageTable::GetPte<PAGE_LEVEL>(const VirtAddr &virtAddr) const;

} // namespace x86_64

} // namespace BartOS
