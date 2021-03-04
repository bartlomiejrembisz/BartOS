#include "Vmm.h"
#include "Kernel/Arch/x86_64/CPU.h"
#include "Kernel/Arch/x86_64/Interrupts/Interrupt.h"

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
    m_kernelHeap.Initialize();

    //! Disable kmalloc eternal forever since we now have a heap.
    disable_kmalloc_eternal();

    m_pKernelAddressSpace = AddressSpace::CreateKernelAddressSpace(&p4_table);
    m_isInitialized = true;

    //kprintf("[VMM] VMM initialized. Pool start=%p, Page count=%u, Page handle size=%u\n", m_kernelAddressSpace.m_pVmAreas,
      //  m_kernelAddressSpace.m_nVmAreas, sizeof(VmArea));

    kprintf("[VMM] Kernel Address Space end: %p\n", (m_pKernelAddressSpace->m_vmAreaList[0]->m_vend.Get()));
    kprintf("[VMM] Memory used by VMM: %u KiB\n", (sizeof(VmArea) * m_pKernelAddressSpace->m_vmAreaList.size()) / KiB);
}

// ---------------------------------------------------------------------------------------------------------

KernelHeap &Vmm::GetKernelHeap()
{
    return m_kernelHeap;
}

// ---------------------------------------------------------------------------------------------------------

void Vmm::KernelInitMap(const PhysicalAddress &physicalAddress, const VirtualAddress &virtualAddress)
{
    ASSERT(!m_isInitialized);
    
    ASSERT(TEMP_MAP_ADDR_BASE > virtualAddress.Get());
    
    //! Get PTE of the p3 table and check if it's present.
    x86_64::PageTableEntry &p4TableEntry = p4_table.GetPte<PML4>(virtualAddress);
    x86_64::PageTable *pP3Table = nullptr;
    ASSERT(p4TableEntry.IsPresent());

    pP3Table = (x86_64::PageTable *) TempMap(p4TableEntry.GetPhysicalAddress(), PML3);

    //! Get PTE of the p3 table and check if it's present.
    x86_64::PageTableEntry &p3TableEntry = pP3Table->GetPte<PML3>(virtualAddress);
    x86_64::PageTable *pP2Table = nullptr;
    ASSERT(p3TableEntry.IsPresent());

    pP2Table = (x86_64::PageTable *) TempMap(p3TableEntry.GetPhysicalAddress(), PML2);

    //! Get PTE of the p2 table.
    x86_64::PageTableEntry &p2TableEntry = pP2Table->GetPte<PML2>(virtualAddress);
    x86_64::PageTable *pP1Table = nullptr;

    ASSERT(ALIGN(physicalAddress.Get(), PAGE_2M) == physicalAddress.Get());
    ASSERT(ALIGN(virtualAddress.Get(), PAGE_2M) == virtualAddress.Get());

    p2TableEntry.SetPhysicalAddress(physicalAddress);
    p2TableEntry.SetPageFlags(PRESENT | WRITABLE | HUGE_PAGE);

    x86_64::Invlpg(virtualAddress);

    return;
}

// ---------------------------------------------------------------------------------------------------------

void *Vmm::TempMap(const PhysicalAddress &physicalAddress, const PageMapLevel pml)
{
    VirtualAddress virtualAddress(TEMP_MAP_ADDR_BASE);
    virtualAddress.SetLevel1(virtualAddress.GetLevel1() + static_cast<uint8_t>(pml));

    x86_64::PageTableEntry &entry = m_pTempMapTable->m_entries[pml];
    entry.SetPhysicalAddress(physicalAddress);
    entry.SetPresent(1);
    entry.SetWritable(1);
    entry.SetUserAccessible(0);
    entry.SetWriteThrough(1);
    entry.SetCacheDisabled(0);
    entry.SetHugePage(0);
    entry.SetGlobal(0);
    entry.SetNoExecute(0);

    x86_64::Invlpg(virtualAddress);

    return static_cast<void *>(virtualAddress);
}

} // namespace MM

} // namespace BartOS