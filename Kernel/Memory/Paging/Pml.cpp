#include "Pml.h"

#error "ignore"

#include "Memory/Vmm.h"

namespace BartOS
{

namespace MM
{

namespace
{

x86_64::PageTable *PhysAddrToPageTable(PhysAddr physAddr)
{
    return static_cast<x86_64::PageTable *>(VirtAddr::FromPhys(physAddr));
}

// ---------------------------------------------------------------------------------------------------------

template<typename T>
void PageTableCallback(const PageTableIndex entryIndex, x86_64::PageTableEntry &pte, void *pData)
{
    using CurrentPml = T;
    using Entry = typename CurrentPml::Entry;

    CurrentPml * const pPml = reinterpret_cast<CurrentPml * const>(pData);

    if (pte.IsPresent())
    {
        Entry *pEntry;
        if constexpr(CurrentPml::m_level == PML1)
            pEntry = new Entry(pte.GetPageFlags());
        else
            pEntry = new Entry(pte, kernel_init_tag());

        pPml->AddEntry(entryIndex, pEntry);
    }
}

template void PageTableCallback<Pml4>(const PageTableIndex entryIndex, x86_64::PageTableEntry &pte, void *pData);
template void PageTableCallback<Pml3>(const PageTableIndex entryIndex, x86_64::PageTableEntry &pte, void *pData);
template void PageTableCallback<Pml2>(const PageTableIndex entryIndex, x86_64::PageTableEntry &pte, void *pData);
template void PageTableCallback<Pml1>(const PageTableIndex entryIndex, x86_64::PageTableEntry &pte, void *pData);

} // namespace

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

PageTableEntry::PageTableEntry(x86_64::PageTableEntry * const pPte) :
    m_pPte(pPte)
{
}

// ---------------------------------------------------------------------------------------------------------

PhysAddr PageTableEntry::GetPhysAddr() const
{
    return m_pPte->GetPhysAddr();
}

// ---------------------------------------------------------------------------------------------------------

PageFlags PageTableEntry::GetPageFlags() const
{
    if (!m_pPte)
        return NO_FLAGS;

    return m_pPte->GetPageFlags();
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
Pml<ENTRY_TYPE, PML>::Pml(x86_64::PageTableEntry &pte) :
    Base(&pte),
    m_pPageTable(MM::Vmm::Get().GetHeapManager().AllocatePml()),
    m_ownsPageTable(true),
    m_subDirectoryMap(m_hasher)
{
}

// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
Pml<ENTRY_TYPE, PML>::Pml(x86_64::PageTable * const pPageTable, kernel_init_tag) :
    Base(nullptr),
    m_pPageTable(pPageTable),
    m_ownsPageTable(false),
    m_subDirectoryMap(m_hasher)
{
    //! Walk the page table and initialize the Pml.
    if (!(m_pageFlags & HUGE_PAGE))
        m_pPageTable->ForEachEntry(PageTableCallback<Type>, reinterpret_cast<void *>(this));
}

// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
Pml<ENTRY_TYPE, PML>::Pml(x86_64::PageTableEntry &pte, kernel_init_tag) :
    Base(&pte),
    m_pPageTable(PhysAddrToPageTable(pte.GetPhysAddr())),
    m_ownsPageTable(false),
    m_subDirectoryMap(m_hasher)
{
    //! Walk the page table and initialize the Pml.
    if (!(m_pageFlags & HUGE_PAGE))
        pPageTable->ForEachEntry(PageTableCallback<Type>, reinterpret_cast<void *>(this));   
}

// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
Pml<ENTRY_TYPE, PML>::~Pml()
{
    if (m_ownsPageTable)
        Vmm::Get().GetHeapManager().FreePml(m_pPageTable);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
void Pml<ENTRY_TYPE, PML>::MapPage(const PhysAddr &physAddr, const VirtAddr &virtAddr,
        const PageFlags pageFlags, const PageSize pageSize)
{
    PageTableIndex index = virtAddr.GetLevel<AddressBitField>();

    typename SubDirectoryMap::iterator it = m_subDirectoryMap.find(index);
    SubDirectory *pSubDirectory = nullptr;

    bool isHugePage = ((pageFlags & HUGE_PAGE));// && (HUGE_PAGE_SIZE == pageSize));


    //! Next level not found - allocate and keep going.
    if (m_subDirectoryMap.end() == it)
    {
        if (isHugePage)
        {

        }
        else
        {
            pSubDirectory = new SubDirectory(PAGE_DIR_FLAGS);
            
        }
        
    }
    else
    {
        pSubDirectory = it->template get<1>();
    }

    ASSERT(pSubDirectory);

    if (!isHugePage)
        pSubDirectory->MapPage(physAddr, virtAddr, pageFlags, pageSize);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ENTRY_TYPE, PageMapLevel PML>
void Pml<ENTRY_TYPE, PML>::AddEntry(const PageTableIndex pageTableIndex, SubDirectory * const pSubDirectory)
{
    m_subDirectoryMap.insert(pageTableIndex, pSubDirectory);
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! Specializations for Pml1 (lowest page table level) pointing to 4K pages.

template <>
void Pml1::MapPage(const PhysAddr &physAddr, const VirtAddr &virtAddr,
        const PageFlags pageFlags, const PageSize pageSize)
{

}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template class Pml<PageEntry, PML1>;
template class Pml<Pml1, PML2>;
template class Pml<Pml2, PML3>;
template class Pml<Pml3, PML4>;

} // namespace MM

} // namespace BartOS
