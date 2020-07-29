#include "PhysicalPage.h"

#include "Pmm.h"

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

void PhysicalPage::OnDie(Parent &object)
{
    PhysicalPage &physicalPage = static_cast<PhysicalPage &>(object);

    Pmm::Get().ReturnPage(&physicalPage);
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

} // namespace MM

} // namespace BartOS
