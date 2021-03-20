#include "AddressSpace.h"

#include "Pmm.h"
#include "Vmm.h"

namespace BartOS
{

namespace MM
{

namespace
{

struct VmAreaDescriptor
{
    VirtAddr          m_vstart { 0 };
    VirtAddr          m_vend { 0 };
    PageFlags         m_pageFlags { NO_FLAGS };
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
template<typename PML_TYPE>
void PageTableCallback(const size_t entryIndex, typename PML_TYPE::Entry &entry, void *pData);

template<typename PML_TYPE>
void PageTableCallback(const size_t entryIndex, typename PML_TYPE::Entry &entry, void *pData)
{
    using Pml = PML_TYPE;
    using CurrentAddressLevel = typename Pml::AddressBitField;
    using Entry = typename PML_TYPE::Entry;

    VmAreaDescriptor * const pVmAreaDescriptor = reinterpret_cast<VmAreaDescriptor *>(pData);

    if constexpr (Pml::m_level == PML2)
    {
        if ((x86_64::PageTable::PAGE_TABLE_COUNT - 1) == entryIndex)
            return;
    }
    
    if (pVmAreaDescriptor->m_vstart.GetLevel<CurrentAddressLevel>() <= entryIndex)
        pVmAreaDescriptor->m_vstart.SetLevel<CurrentAddressLevel>(entryIndex);

    if (entry.GetPageFlags() & HUGE_PAGE)
    {
        pVmAreaDescriptor->m_vend += Pml::m_pageSize;
        pVmAreaDescriptor->m_pageFlags = entry.GetPageFlags();
    }
    else
    {   
        if constexpr (Pml::m_level != PML1)
        {
            entry.ForEach(PageTableCallback<Entry>, pData);
        }
        else
        {
            pVmAreaDescriptor->m_vend += Pml::m_pageSize;
            pVmAreaDescriptor->m_pageFlags = entry.GetPageFlags();
        }
    }
}

template void PageTableCallback<Pml4>(const size_t entryIndex, typename Pml4::Entry &entry, void *pData);
template void PageTableCallback<Pml3>(const size_t entryIndex, typename Pml3::Entry &entry, void *pData);
template void PageTableCallback<Pml2>(const size_t entryIndex, typename Pml2::Entry &entry, void *pData);
template void PageTableCallback<Pml1>(const size_t entryIndex, typename Pml1::Entry &entry, void *pData);

} // namespace

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

AddressSpace::AddressSpace(x86_64::PageTable * const pPageTable, kernel_init_tag) :
    m_pml4(pPageTable, kernel_init_tag()),
    m_textVmArea(),
    m_roDataVmArea(),
    m_dataVmArea(),
    m_bssVmArea(),
    m_heapVmArea(),
    m_stackVmArea()
{
    VmArea *pMultibootVmArea = new VmArea(VmAreaType::EXE_SECTION, (Address_t) __multiboot_start, (Address_t) __multiboot_end, *pKernelAddressSpace);
    pMultibootVmArea->m_pageFlags = (PRESENT | NO_EXECUTE);
    pMultibootVmArea->m_vmAreaFlags = (VmAreaFlags::IS_CONTIGUOUS);
    size_t nPages = (pMultibootVmArea->m_vend - pMultibootVmArea->m_vstart).Get() / Page::m_pageSize;
    pMultibootVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(PhysAddr(pMultibootVmArea->m_vstart), nPages));
    pKernelAddressSpace->AddVmArea(pMultibootVmArea);

    //! Text region
    VmArea *pTextVmArea = new VmArea(VmAreaType::EXE_SECTION, (Address_t) __text_start, (Address_t) __text_end, *pKernelAddressSpace);
    pTextVmArea->m_pageFlags = (WRITABLE);
    pTextVmArea->m_vmAreaFlags = (VmAreaFlags::EXECUTABLE | VmAreaFlags::IS_CONTIGUOUS);
    nPages = (pTextVmArea->m_vend - pTextVmArea->m_vstart).Get() / Page::m_pageSize;
    pTextVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(PhysAddr(pTextVmArea->m_vstart), nPages));
    pKernelAddressSpace->AddVmArea(pTextVmArea);

    //! RoData region.
    VmArea *pRoDataVmArea = new VmArea(VmAreaType::EXE_SECTION, (Address_t) __rodata_start, (Address_t) __rodata_end, *pKernelAddressSpace);
    pRoDataVmArea->m_pageFlags = (PRESENT | NO_EXECUTE);
    pRoDataVmArea->m_vmAreaFlags = (VmAreaFlags::IS_CONTIGUOUS);
    nPages = (pRoDataVmArea->m_vend - pRoDataVmArea->m_vstart).Get() / Page::m_pageSize;
    pRoDataVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(PhysAddr(pRoDataVmArea->m_vstart), nPages));
    pKernelAddressSpace->AddVmArea(pRoDataVmArea);
    
    //! Data region.
    VmArea *pDataVmArea = new VmArea(VmAreaType::EXE_SECTION, (Address_t) __data_start, (Address_t) __data_end, *pKernelAddressSpace);
    pDataVmArea->m_pageFlags = (PRESENT | NO_EXECUTE);
    pDataVmArea->m_vmAreaFlags = (VmAreaFlags::IS_CONTIGUOUS);
    nPages = (pDataVmArea->m_vend - pDataVmArea->m_vstart).Get() / Page::m_pageSize;
    pDataVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(PhysAddr(pDataVmArea->m_vstart), nPages));
    pKernelAddressSpace->AddVmArea(pDataVmArea);

    //! Data region.
    VmArea *pBssVmArea = new VmArea(VmAreaType::EXE_SECTION, (Address_t) __bss_start, (Address_t) __bss_end, *pKernelAddressSpace);
    pBssVmArea->m_pageFlags = (PRESENT | NO_EXECUTE);
    pBssVmArea->m_vmAreaFlags = (VmAreaFlags::IS_CONTIGUOUS);
    nPages = (pBssVmArea->m_vend - pBssVmArea->m_vstart).Get() / Page::m_pageSize;
    pBssVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(PhysAddr(pBssVmArea->m_vstart), nPages));
    pKernelAddressSpace->AddVmArea(pBssVmArea);

    //! Eternal heap area.
    VmArea *pEternalHeapVmArea = new VmArea(VmAreaType::PERMANENT, (Address_t) get_kmalloc_eternal_begin(), (Address_t) get_kmalloc_eternal_ptr(), *pKernelAddressSpace);
    pEternalHeapVmArea->m_pageFlags = (PRESENT | WRITABLE);
    pEternalHeapVmArea->m_vmAreaFlags = (VmAreaFlags::IS_CONTIGUOUS);
    nPages = (pEternalHeapVmArea->m_vend - pEternalHeapVmArea->m_vstart).Get() / Page::m_pageSize;
    pEternalHeapVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(PhysAddr(pEternalHeapVmArea->m_vstart), nPages));
    pKernelAddressSpace->AddVmArea(pEternalHeapVmArea);

    //! Heap starts at a 4K page aligned address after the kmalloc eternal buffer.
    const VirtAddr heapStart((Address_t) get_kmalloc_eternal_ptr());
    VmArea *pHeapVmArea = new VmArea(VmAreaType::HEAP, heapStart, heapStart + (Address_t) INITIAL_HEAP_SIZE, *pKernelAddressSpace);
    pHeapVmArea->m_pageFlags = (WRITABLE);
    pHeapVmArea->m_vmAreaFlags = (VmAreaFlags::CAN_GROW);
    nPages = (pHeapVmArea->m_vend - pHeapVmArea->m_vstart).Get() / Page::m_pageSize;
    pHeapVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(nPages));
    pKernelAddressSpace->AddVmArea(pHeapVmArea);

    //! Stack area.
    VmArea *pStackVmArea = new VmArea(VmAreaType::STACK, VirtAddr(Vmm::TEMP_MAP_ADDR_BASE - INITIAL_STACK_SIZE), VirtAddr(Vmm::TEMP_MAP_ADDR_BASE), *pKernelAddressSpace);
    pStackVmArea->m_pageFlags = (WRITABLE);
    pStackVmArea->m_vmAreaFlags = (VmAreaFlags::CAN_GROW | VmAreaFlags::GROWS_DOWN);
    nPages = (pStackVmArea->m_vend - pStackVmArea->m_vstart).Get() / Page::m_pageSize;
    pStackVmArea->m_physicalRegionList.push_back(Pmm::Get().AllocateRegion(nPages));
    pKernelAddressSpace->AddVmArea(pStackVmArea);

    //! Temporary mapping area.
    VmArea *pTempMapVmArea = new VmArea(VmAreaType::TEMP_MAPPING, VirtAddr(Vmm::TEMP_MAP_ADDR_BASE), VirtAddr(ADDRESS_SPACE_TOP), *pKernelAddressSpace);
    pTempMapVmArea->m_pageFlags = (WRITABLE);
    pTempMapVmArea->m_vmAreaFlags = (VmAreaFlags::NO_FLAGS);
    pKernelAddressSpace->AddVmArea(pTempMapVmArea);
}

// ---------------------------------------------------------------------------------------------------------

void *AddressSpace::Allocate(size_t nBytes, const PageSize pageSize, const PageFlags pageFlags)
{
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------

void AddressSpace::AddVmArea(VmArea *pVmArea)
{
    m_vmAreaList.push_back(pVmArea);
}

// ---------------------------------------------------------------------------------------------------------

void AddressSpace::SynchronizePml()
{
}

// ---------------------------------------------------------------------------------------------------------

AddressSpace *AddressSpace::CreateKernelAddressSpace(x86_64::PageTable * const pPageTable)
{
    AddressSpace * const pKernelAddressSpace = new AddressSpace(pPageTable);

    

    pKernelAddressSpace->SynchronizePml();

    return pKernelAddressSpace;
}

} // namespace MM

} // namespace BartOS
