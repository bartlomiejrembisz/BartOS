#include "VirtualPage.h"
#include "AddressSpace.h"

namespace BartOS
{

namespace MM
{

VirtualPage::VirtualPage() :
    m_addr(0),
    m_pAddressSpace(nullptr),
    m_pPte(nullptr)
{
}

// ---------------------------------------------------------------------------------------------------------

VirtualPage::VirtualPage(const VirtualAddress vaddr, AddressSpace &addressSpace, MM::PageTableEntry * const pPte,
        const MemoryPool::PhysicalRange &physicalRange, const PageSize pageSize) :
    VirtualPage()
{
    Initialize(vaddr, addressSpace, pPte, physicalRange, pageSize);
}

// ---------------------------------------------------------------------------------------------------------

void VirtualPage::Initialize(const VirtualAddress vaddr, AddressSpace &addressSpace, MM::PageTableEntry * const pPte,
    const MemoryPool::PhysicalRange &physicalRange, const PageSize pageSize)
{
    m_addr = vaddr;
    m_pAddressSpace = &addressSpace;
    m_pPte = pPte;
    m_physicalRange = physicalRange;
    m_pageSize = pageSize;
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress VirtualPage::GetAddress() const
{
    return m_addr;
}

} // namespace MM

} // namespace BartOS
