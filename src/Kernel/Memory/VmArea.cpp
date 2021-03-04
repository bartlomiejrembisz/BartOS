#include "VmArea.h"
#include "AddressSpace.h"

namespace BartOS
{

namespace MM
{

VmArea::VmArea(const VmAreaType vmAreaType, const VirtualAddress vstart, const VirtualAddress vend, AddressSpace &addressSpace) :
    m_vmAreaType(vmAreaType),
    m_vstart(vstart),
    m_vend(vend),
    m_addressSpace(addressSpace),
    m_pageFlags(NO_FLAGS),
    m_vmAreaFlags(VmAreaFlags::NO_FLAGS),
    m_pageSize(PAGE_4K)
{
}

// ---------------------------------------------------------------------------------------------------------

VmArea::~VmArea()
{
}

// ---------------------------------------------------------------------------------------------------------

void VmArea::SetPageFlags(const PageFlags pageFlags)
{
    m_pageFlags |= pageFlags;
}

// ---------------------------------------------------------------------------------------------------------

void VmArea::UnsetPageFlags(const PageFlags pageFlags)
{
    m_pageFlags &= ~pageFlags;
}

// ---------------------------------------------------------------------------------------------------------

void VmArea::SetPageFlags(const VmAreaFlags vmAreaFlags)
{
    m_vmAreaFlags |= vmAreaFlags;
}

// ---------------------------------------------------------------------------------------------------------

void VmArea::UnsetPageFlags(const VmAreaFlags vmAreaFlags)
{
    m_vmAreaFlags &= ~vmAreaFlags;
}

} // namespace MM

} // namespace BartOS
