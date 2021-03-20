#include "Page.h"

#include "Pmm.h"

namespace BartOS
{

namespace MM
{

Page::Page(const PhysAddr paddr) :
    m_addr(paddr)
{
}

// ---------------------------------------------------------------------------------------------------------

Page::Page(Page &&rhs) : 
    m_addr(std::move(rhs.m_addr))
{
    rhs.m_addr.Set(0);
}

// ---------------------------------------------------------------------------------------------------------

Page &Page::operator=(Page &&rhs)
{
    Parent::operator=(std::forward<Parent &&>(rhs));
    m_addr = std::move(rhs.m_addr);

    return *this;
}

// ---------------------------------------------------------------------------------------------------------

void Page::OnDie(Page &page)
{
    Pmm::Get().ReturnPage(&page);
}

// ---------------------------------------------------------------------------------------------------------

PhysAddr Page::GetAddress() const
{
    return m_addr;
}

} // namespace MM

} // namespace BartOS
