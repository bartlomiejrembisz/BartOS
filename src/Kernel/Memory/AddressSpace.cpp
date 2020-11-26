#include "AddressSpace.h"

#include "Pmm.h"
#include "Vmm.h"

namespace BartOS
{

namespace MM
{

AddressSpace::AddressSpace() :
    m_pPageTable(nullptr)
{
}

// ---------------------------------------------------------------------------------------------------------

AddressSpace::AddressSpace(MM::PageTable * const pPageTable) :
    m_pPageTable(pPageTable),
    m_addressBreak(0)
{
}

// ---------------------------------------------------------------------------------------------------------

AddressSpace::~AddressSpace()
{
}

} // namespace MM

} // namespace BartOS
