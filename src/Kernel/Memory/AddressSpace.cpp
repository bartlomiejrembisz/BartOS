#include "AddressSpace.h"

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
    m_pPageTable(pPageTable)
{
}

// ---------------------------------------------------------------------------------------------------------

AddressSpace::~AddressSpace()
{
}

} // namespace MM

} // namespace BartOS
