#include "KernelAddressSpace.h"

#include "Pmm.h"
#include "Vmm.h"

#include "Libraries/libc/string.h"

#include <new>

namespace BartOS
{

namespace MM
{

const VirtualAddress KernelAddressSpace::KERNEL_BEGIN_ADDRESS((Address_t) __kernel_virtual_start);

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

namespace
{

struct KernelMappingInfo
{
    VirtualAddress vstart;
    VirtualAddress vend;
};

/*
 *  @brief Page table callback.
 *
 *  @param vstart the start of the kernel VM area.
 *  @param vend the end of the kernel VM area.
 *  @param 
 * 
 *  @return pointer to the PTE
 */
template<PageTableLevel ptl>
void PageTableCallback(const size_t entryIndex, PageTableEntry &pte, void *pData);

template<PageTableLevel ptl>
void PageTableCallback(const size_t entryIndex, PageTableEntry &pte, void *pData)
{
    using CurrentAddressLevel = typename PageLevelToAddrLevel<ptl>::Level;

    KernelMappingInfo * const pKernelMappingInfo = reinterpret_cast<KernelMappingInfo *>(pData);

    if constexpr (ptl == TABLE_LEVEL2)
    {
        if ((PageTable::PAGE_TABLE_COUNT - 1) == entryIndex)
            return;
    }
    
    if (pte.IsPresent())
    {
        if (pKernelMappingInfo->vstart.GetLevel<CurrentAddressLevel>() > entryIndex)
            pKernelMappingInfo->vstart.SetLevel<CurrentAddressLevel>(entryIndex);
        
        pKernelMappingInfo->vend.SetLevel<CurrentAddressLevel>(entryIndex);

        //! Can't go lower than TABLE_LEVEL1 since that entry points to a 4K page.
        if constexpr (ptl != TABLE_LEVEL1)
        {
            if (!pte.IsHugePage())
            {
                constexpr PageTableLevel lowerLever = LowerPageLevel<ptl>::m_level;

                PageTable *pLowerPageTable = static_cast<PageTable *>(VirtualAddress::Create(pte.GetPhysicalAddress()));
                pLowerPageTable->ForEachEntry(PageTableCallback<lowerLever>, pData);
            }
            else
            {
                pKernelMappingInfo->vend += PAGE_2M;
            }
            
        }
    }
}

template void PageTableCallback<TABLE_LEVEL4>(const size_t entryIndex, PageTableEntry &pte, void *pData);
template void PageTableCallback<TABLE_LEVEL3>(const size_t entryIndex, PageTableEntry &pte, void *pData);
template void PageTableCallback<TABLE_LEVEL2>(const size_t entryIndex, PageTableEntry &pte, void *pData);
template void PageTableCallback<TABLE_LEVEL1>(const size_t entryIndex, PageTableEntry &pte, void *pData);

} // namespace

// ---------------------------------------------------------------------------------------------------------

KernelAddressSpace::KernelAddressSpace(MM::PageTable * const pPageTable) :
    AddressSpace(pPageTable)
{
}

// ---------------------------------------------------------------------------------------------------------

void *KernelAddressSpace::Allocate(size_t nBytes, const PageSize pageSize, const PageFlags pageFlags)
{

    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------

void KernelAddressSpace::Initialize()
{
    m_pVMAreaPtr = static_cast<VMArea **>(kmalloc_eternal(sizeof(VMArea * ) * MAX_SMALL_KERNEL_PAGES));
    memset(m_pVMAreaPtr, 0, sizeof(VMArea * ) * MAX_SMALL_KERNEL_PAGES);

    SynchronizeKernelAddressSpace();

    m_addressBreak = reinterpret_cast<Address_t>(get_kmalloc_eternal_ptr());
}

// ---------------------------------------------------------------------------------------------------------

void KernelAddressSpace::SynchronizeKernelAddressSpace()
{
    //! Synchronize the kernel address space.
    //! Iterate over kernel page tables.

    KernelMappingInfo kernelMappingInfo;
    kernelMappingInfo.vstart = VirtualAddress::Create(PhysicalAddress(0));
    kernelMappingInfo.vend = VirtualAddress((Address_t)__kernel_virtual_end).PageAddress(PAGE_2M);
    m_pPageTable->ForEachEntry(PageTableCallback<TABLE_LEVEL4>, reinterpret_cast<void *>(&kernelMappingInfo));

    VMArea &VMArea = m_kernelVMArea;
    m_pVMAreaPtr[0] = &m_kernelVMArea;
    ++m_nVMAreas;

    VMArea.m_vstart = kernelMappingInfo.vstart;
    VMArea.m_vend = kernelMappingInfo.vend;
    VMArea.m_pAddressSpace = this;

    VMArea.Initialize(kernelMappingInfo.vstart, kernelMappingInfo.vend, *this, HUGE_PAGE);

    size_t VMAreaSize = (PhysicalAddress::Create(VMArea.m_vend) - PhysicalAddress::Create(VMArea.m_vstart)).Get();

    const size_t nPhysicalPages = VMAreaSize / PhysicalPage::m_pageSize;

    void *pMalloc = get_kmalloc_eternal_ptr();

    VMArea.m_physicalRange = (std::move(Pmm::Get().AllocateRange(PhysicalAddress::Create(VMArea.m_vstart), nPhysicalPages)));

    /*
    //! Kernel boots up in huge page mode only for now.
    for (size_t pageIndex = 0; pageIndex < MAX_HUGE_KERNEL_PAGES; ++pageIndex)
    {
        const VirtualAddress virtualAddress(PageIndexToAddress(pageIndex, PAGE_2M));

        // Stop allocating pages once the entire kernel space is allocated.
        if (virtualAddress >= get_kmalloc_eternal_ptr())
            break;

        PageTableEntry *pP1TableEntry = FindPageTableEntry(virtualAddress);
        if (!pP1TableEntry)
            ASSERT(false); // Impossible to fail here.

        VMArea &VMArea = m_pVMAreas[pageIndex];
        MemoryPool::PhysicalRange physicalRange(std::move(Pmm::Get().AllocateRange(PhysicalAddress::Create(virtualAddress), PAGE_2M / PhysicalPage::m_pageSize)));

        //VMArea.Initialize(virtualAddress, *this);
        ++m_nVMAreas;
    }*/ 
}

// ---------------------------------------------------------------------------------------------------------

MM::PageTableEntry *KernelAddressSpace::FindPageTableEntry(const VirtualAddress vAddr)
{
    // Get p3 table entry.
    PageTableEntry &p3TableEntry = m_pPageTable->GetPte<TABLE_LEVEL4>(vAddr);
    PageTable *pP3Table = nullptr;

    if (!p3TableEntry.IsPresent())
        return nullptr;

    //! HACK: translate physical address to virtual because it's the kernel space.
    pP3Table = static_cast<PageTable *>(VirtualAddress::Create(p3TableEntry.GetPhysicalAddress()));

    // Get p2 table entry.
    PageTableEntry &p2TableEntry = pP3Table->GetPte<TABLE_LEVEL3>(vAddr);
    PageTable *pP2Table = nullptr;

    if (!p2TableEntry.IsPresent())
        return nullptr;
    
    //! HACK: translate physical address to virtual because it's the kernel space.
    pP2Table = static_cast<PageTable *>(VirtualAddress::Create(p2TableEntry.GetPhysicalAddress()));

    // Return p1 table entry.
    return &pP2Table->GetPte<TABLE_LEVEL2>(vAddr);
}

// ---------------------------------------------------------------------------------------------------------

bool KernelAddressSpace::IsKernelAddress(const VirtualAddress vAddr)
{
    return (vAddr >= KERNEL_BEGIN_ADDRESS) && (vAddr.Get() < TEMP_MAP_ADDR_BASE);
}

} // namespace MM

} // namespace BartOS
