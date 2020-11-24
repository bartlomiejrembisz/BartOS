#include "KernelAddressSpace.h"

#include "Pmm.h"
#include "Vmm.h"

#include "Libraries/libc/string.h"

namespace BartOS
{

namespace MM
{

KernelAddressSpace::KernelAddressSpace(MM::PageTable * const pPageTable) :
    AddressSpace(pPageTable)
{
}

// ---------------------------------------------------------------------------------------------------------

VirtualPage *KernelAddressSpace::GetVirtualPage(const VirtualAddress virtualAddress)
{
    const Address_t relocatedAddress = virtualAddress.PageAddress(PAGE_2M).Get() - (Address_t)KERNEL_VMA;
    if (relocatedAddress > GetMaximumKernelAddressSpaceSize())
        ASSERT(false);

    const size_t pageIndex = static_cast<size_t>(relocatedAddress / PAGE_2M);
    return &m_pVirtualPages[pageIndex];
}

// ---------------------------------------------------------------------------------------------------------

void KernelAddressSpace::Initialize()
{
    m_nVirtualPages = Max(static_cast<size_t>(MINIMUM_KERNEL_PAGES), (Pmm::Get().GetMemoryStats().m_totalMemory / KERNEL_MEMORY_FACTOR) / KERNEL_PAGE_SIZE);
    m_pVirtualPages = static_cast<VirtualPage *>(kmalloc_eternal(m_nVirtualPages));

    SynchronizeKernelAddressSpace();
}

// ---------------------------------------------------------------------------------------------------------

void KernelAddressSpace::SynchronizeKernelAddressSpace()
{
    for (size_t pageIndex = 0; pageIndex < m_nVirtualPages; ++pageIndex)
    {
        const VirtualAddress virtualAddress((pageIndex * PAGE_2M) + (Address_t)KERNEL_VMA);

        // Stop allocating pages once the entire kernel space is allocated.
        if (virtualAddress >= get_kmalloc_eternal_ptr())
            break;

        // Get p3 table entry
        PageTableEntry &p3TableEntry = m_pPageTable->GetPte<TABLE_LEVEL4>(virtualAddress);
        PageTable *pP3Table = nullptr;

        //! Should never happen.
        if (!p3TableEntry.IsPresent())
        {
            //! Allocate p3 table.
            p3TableEntry.SetPhysicalAddress(PhysicalAddress::Create(
                VirtualAddress(kmalloc_eternal_aligned(sizeof(PageTable), PAGE_SIZE))));

            //! Set the entry flags.
            p3TableEntry.SetPresent(1);
            p3TableEntry.SetWritable(1);
            p3TableEntry.SetUserAccessible(0);
            p3TableEntry.SetWriteThrough(1);
            p3TableEntry.SetCacheDisabled(0);
            p3TableEntry.SetHugePage(0);
            p3TableEntry.SetGlobal(1);
            p3TableEntry.SetNoExecute(0);
        }

        //! HACK: translate physical address to virtual because it's guaranteed the page is mapped.
        pP3Table = static_cast<PageTable *>(VirtualAddress::Create(p3TableEntry.GetPhysicalAddress()));

        // Get p2 table entry
        PageTableEntry &p2TableEntry = pP3Table->GetPte<TABLE_LEVEL3>(virtualAddress);
        PageTable *pP2Table = nullptr;

        //! Should never happen.
        if (!p2TableEntry.IsPresent())
        {
            //! Allocate p2 table.
            p2TableEntry.SetPhysicalAddress(PhysicalAddress::Create(
                VirtualAddress(kmalloc_eternal_aligned(sizeof(PageTable), PAGE_SIZE))));

            //! Set the entry flags.
            p2TableEntry.SetPresent(1);
            p2TableEntry.SetWritable(1);
            p2TableEntry.SetUserAccessible(0);
            p2TableEntry.SetWriteThrough(1);
            p2TableEntry.SetCacheDisabled(0);
            p2TableEntry.SetHugePage(0);
            p2TableEntry.SetGlobal(1);
            p2TableEntry.SetNoExecute(0);
        }

        //! HACK: translate physical address to virtual because it's guaranteed the page is mapped.
        pP2Table = static_cast<PageTable *>(VirtualAddress::Create(p2TableEntry.GetPhysicalAddress()));

        // Get p1 table entry
        PageTableEntry &p1TableEntry = pP2Table->GetPte<TABLE_LEVEL2>(virtualAddress);

        //! Should never happen.
        if (!p1TableEntry.IsPresent())
        {
            //! Allocate p1 table.
            p1TableEntry.SetPhysicalAddress(PhysicalAddress::Create(
                VirtualAddress(kmalloc_eternal_aligned(sizeof(PageTable), PAGE_SIZE))));

            //! Set the entry flags.
            p1TableEntry.SetPresent(1);
            p1TableEntry.SetWritable(1);
            p1TableEntry.SetUserAccessible(0);
            p1TableEntry.SetWriteThrough(1);
            p1TableEntry.SetCacheDisabled(0);
            p1TableEntry.SetHugePage(0);
            p1TableEntry.SetGlobal(1);
            p1TableEntry.SetNoExecute(0);
        }

        VirtualPage &virtualPage = m_pVirtualPages[pageIndex];
        MemoryPool::PhysicalRange physicalRange = Pmm::Get().AllocateRange(PhysicalAddress::Create(virtualAddress), PAGE_2M / PhysicalPage::m_pageSize);
        virtualPage.Initialize(virtualAddress, *this, &p1TableEntry, std::move(physicalRange), PAGE_2M);
    }
}

} // namespace MM

} // namespace BartOS
