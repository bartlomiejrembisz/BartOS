#include "VirtAddr.h"

#include "PhysAddr.h"

#include "Memory.h"

namespace BartOS
{

VirtAddr::VirtAddr()
{
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::VirtAddr(const Address_t address) : 
    Address<VirtAddr>(address)
{
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level4::ValueType VirtAddr::SetLevel4(const Layout::Level4::ValueType level4)
{
    Layout::Level4::Set(m_addr, level4);

    return level4;
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level3::ValueType VirtAddr::SetLevel3(const Layout::Level3::ValueType level3)
{
    Layout::Level3::Set(m_addr, level3);
    
    return level3;
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level2::ValueType VirtAddr::SetLevel2(const Layout::Level2::ValueType level2)
{
    Layout::Level2::Set(m_addr, level2);
    
    return level2;
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level1::ValueType VirtAddr::SetLevel1(const Layout::Level1::ValueType level1)
{
    Layout::Level1::Set(m_addr, level1);
    
    return level1;
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::PageOffset::ValueType VirtAddr::SetPageOffset(const Layout::PageOffset::ValueType pageOffset)
{
    Layout::PageOffset::Set(m_addr, pageOffset);
    
    return pageOffset;
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level4::ValueType VirtAddr::GetLevel4() const
{
    return Layout::Level4::Get(m_addr);
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level3::ValueType VirtAddr::GetLevel3() const
{
    return Layout::Level3::Get(m_addr);
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level2::ValueType VirtAddr::GetLevel2() const
{
    return Layout::Level2::Get(m_addr);
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::Level1::ValueType VirtAddr::GetLevel1() const
{
    return Layout::Level1::Get(m_addr);
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr::Layout::PageOffset::ValueType VirtAddr::GetPageOffset() const
{
    return Layout::PageOffset::Get(m_addr);
}

// ---------------------------------------------------------------------------------------------------------

VirtAddr VirtAddr::FromPhys(const PhysAddr &physAddr)
{
    return VirtAddr(physAddr.Get() + reinterpret_cast<Address_t>(KERNEL_VMA));
}


} // namespace BartOS
