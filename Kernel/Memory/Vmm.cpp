#include "Vmm.h"
#include "Arch/x86_64/CPU.h"
#include "Arch/x86_64/Interrupts/Interrupt.h"

#include "AddressSpace.h"
#include "Pmm.h"

//! Initial P4 table p4_table symbol exposed from boot.asm
extern "C" BartOS::x86_64::PageTable p4_table;

//! A P1 level table 
extern "C" BartOS::x86_64::PageTable p1_temp_map_table;

namespace BartOS
{

namespace MM
{

x86_64::PageTable * const Vmm::m_pTempMapTable = &p1_temp_map_table;

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

Vmm::Vmm() :
    m_pKernelAddressSpace(nullptr),
    m_pageFaultHandler(*this),
    m_isInitialized(false)
{
    RegisterInterrupt(&m_pageFaultHandler);
}

// ---------------------------------------------------------------------------------------------------------

void Vmm::Initialize()
{
    m_HeapManager.Initialize();

    //! Disable kmalloc eternal forever since we now have a heap.
    disable_kmalloc_eternal();

    m_pKernelAddressSpace = AddressSpace::CreateKernelAddressSpace(&p4_table);
    m_isInitialized = true;

    //kprintf("[VMM] VMM initialized. Pool start=%p, Page count=%u, Page handle size=%u\n", m_kernelAddressSpace.m_pVmAreas,
      //  m_kernelAddressSpace.m_nVmAreas, sizeof(VmArea));

    // kprintf("[VMM] Kernel Address Space end: %p\n", (m_pKernelAddressSpace->m_vmAreaList[0]->m_vend.Get()));
    // kprintf("[VMM] Memory used by VMM: %u KiB\n", (sizeof(VmArea) * m_pKernelAddressSpace->m_vmAreaList.size()) / KiB);
}

// ---------------------------------------------------------------------------------------------------------

HeapManager &Vmm::GetHeapManager()
{
    return m_HeapManager;
}

// ---------------------------------------------------------------------------------------------------------

void Vmm::KernelInitMap(const PhysAddr &physAddr, const VirtAddr &virtAddr)
{
    ASSERT(!m_isInitialized);
    
    ASSERT(TEMP_MAP_ADDR_BASE > virtAddr.Get());
    
    //! Get PTE of the p3 table and check if it's present.
    x86_64::PageTableEntry &p4TableEntry = p4_table.GetPte<PML4>(virtAddr);
    x86_64::PageTable *pP3Table = nullptr;
    ASSERT(p4TableEntry.IsPresent());

    pP3Table = (x86_64::PageTable *) TempMap(p4TableEntry.GetPhysAddr(), PML3);

    //! Get PTE of the p3 table and check if it's present.
    x86_64::PageTableEntry &p3TableEntry = pP3Table->GetPte<PML3>(virtAddr);
    x86_64::PageTable *pP2Table = nullptr;
    ASSERT(p3TableEntry.IsPresent());

    pP2Table = (x86_64::PageTable *) TempMap(p3TableEntry.GetPhysAddr(), PML2);

    //! Get PTE of the p2 table.
    x86_64::PageTableEntry &p2TableEntry = pP2Table->GetPte<PML2>(virtAddr);
    x86_64::PageTable *pP1Table = nullptr;

    ASSERT(ALIGN(physAddr.Get(), PAGE_2M) == physAddr.Get());
    ASSERT(ALIGN(virtAddr.Get(), PAGE_2M) == virtAddr.Get());

    p2TableEntry.SetPhysAddr(physAddr);
    p2TableEntry.SetPageFlags(PRESENT | WRITABLE | HUGE_PAGE);

    x86_64::Invlpg(virtAddr);

    return;
}

// ---------------------------------------------------------------------------------------------------------

void *Vmm::TempMap(const PhysAddr &physAddr, const PageMapLevel pml)
{
    VirtAddr virtAddr(TEMP_MAP_ADDR_BASE);
    virtAddr.SetLevel1(virtAddr.GetLevel1() + static_cast<uint8_t>(pml));

    x86_64::PageTableEntry &entry = m_pTempMapTable->m_entries[pml];
    entry.SetPhysAddr(physAddr);
    entry.SetPresent(1);
    entry.SetWritable(1);
    entry.SetUserAccessible(0);
    entry.SetWriteThrough(1);
    entry.SetCacheDisabled(0);
    entry.SetHugePage(0);
    entry.SetGlobal(0);
    entry.SetNoExecute(0);

    x86_64::Invlpg(virtAddr);

    return static_cast<void *>(virtAddr);
}

} // namespace MM

} // namespace BartOS