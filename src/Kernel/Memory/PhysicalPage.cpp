#include "PhysicalPage.h"

namespace BartOS
{

namespace MM
{

PhysicalPage::PhysicalPage(const PhysicalAddress paddr) :
    m_addr(paddr)
{
}

// ---------------------------------------------------------------------------------------------------------

PhysicalPage::PhysicalPage(PhysicalPage &&rhs) : 
    m_addr(std::move(rhs.m_addr))
{
    rhs.m_addr.Set(0);
}

// ---------------------------------------------------------------------------------------------------------

PhysicalPage &PhysicalPage::operator=(PhysicalPage &&rhs)
{
    Parent::operator=(std::forward<Parent &&>(rhs));
    m_addr = std::move(rhs.m_addr);

    return *this;
}

// ---------------------------------------------------------------------------------------------------------

PhysicalAddress PhysicalPage::GetAddress() const
{
    return m_addr;
}

// ---------------------------------------------------------------------------------------------------------

constexpr uint16_t PhysicalPage::GetSize()
{
    return m_pageSize;
}

// ---------------------------------------------------------------------------------------------------------

void PhysicalPage::OnDie(const Parent &object)
{
    // noop for now.
}

} // namespace MM

} // namespace BartOS
