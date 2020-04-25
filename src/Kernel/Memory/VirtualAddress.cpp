#include "VirtualAddress.h"

#include "PhysicalAddress.h"

#include "Memory.h"

namespace BartOS
{

VirtualAddress::VirtualAddress()
{
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::VirtualAddress(const uintptr_t address) : 
    Address<VirtualAddress>(address)
{
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level4::ValueType VirtualAddress::SetLevel4(const Layout::Level4::ValueType level4)
{
    Layout::Level4::Set(m_address, level4);

    return level4;
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level3::ValueType VirtualAddress::SetLevel3(const Layout::Level3::ValueType level3)
{
    Layout::Level3::Set(m_address, level3);
    
    return level3;
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level2::ValueType VirtualAddress::SetLevel2(const Layout::Level2::ValueType level2)
{
    Layout::Level2::Set(m_address, level2);
    
    return level2;
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level1::ValueType VirtualAddress::SetLevel1(const Layout::Level1::ValueType level1)
{
    Layout::Level1::Set(m_address, level1);
    
    return level1;
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::PageOffset::ValueType VirtualAddress::SetPageOffset(const Layout::PageOffset::ValueType pageOffset)
{
    Layout::PageOffset::Set(m_address, pageOffset);
    
    return pageOffset;
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level4::ValueType VirtualAddress::GetLevel4() const
{
    return Layout::Level4::Get(m_address);
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level3::ValueType VirtualAddress::GetLevel3() const
{
    return Layout::Level3::Get(m_address);
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level2::ValueType VirtualAddress::GetLevel2() const
{
    return Layout::Level2::Get(m_address);
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::Level1::ValueType VirtualAddress::GetLevel1() const
{
    return Layout::Level1::Get(m_address);
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress::Layout::PageOffset::ValueType VirtualAddress::GetPageOffset() const
{
    return Layout::PageOffset::Get(m_address);
}

// ---------------------------------------------------------------------------------------------------------

VirtualAddress VirtualAddress::Create(const PhysicalAddress &physicalAddress)
{
    return VirtualAddress(physicalAddress.Get() + reinterpret_cast<uintptr_t>(KERNEL_VMA));
}


} // namespace BartOS
