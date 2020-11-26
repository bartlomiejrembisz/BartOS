#include "VMArea.h"
#include "AddressSpace.h"

namespace BartOS
{

namespace MM
{

VMArea::VMArea() :
    m_vstart(0),
    m_vend(0),
    m_pAddressSpace(nullptr),
    m_flags(NO_FLAGS),
    m_vmAreaType(PERMANENT)
{
}

// ---------------------------------------------------------------------------------------------------------

VMArea::~VMArea()
{
}

// ---------------------------------------------------------------------------------------------------------

void VMArea::Initialize(const VirtualAddress vstart, const VirtualAddress vend, AddressSpace &addressSpace, const PageFlags pageFlags)
{
    m_vstart = vstart;
    m_vend = vend;
    m_pAddressSpace = &addressSpace;
    m_flags = pageFlags;
}

// ---------------------------------------------------------------------------------------------------------

void VMArea::SetFlag(const PageFlags pageFlags)
{
    m_flags |= pageFlags;
}

// ---------------------------------------------------------------------------------------------------------

void VMArea::UnsetFlag(const PageFlags pageFlags)
{
    m_flags &= ~pageFlags;
}

} // namespace MM

} // namespace BartOS
