#include "PhysicalRegion.h"

namespace BartOS
{

namespace MM
{

PhysicalRegion::PhysicalRegion(const PhysicalRegion &rhs) :
    m_pageList(rhs.m_pageList)
{
}

// ---------------------------------------------------------------------------------------------------------

PhysicalRegion::PhysicalRegion(PhysicalRegion &&rhs) :
    m_pageList(rhs.m_pageList)
{
    rhs.Clear();
}

// ---------------------------------------------------------------------------------------------------------

PhysicalRegion &PhysicalRegion::operator=(const PhysicalRegion &rhs)
{
    Clear();
    
    m_pageList = rhs.m_pageList;

    return *this;
}

// ---------------------------------------------------------------------------------------------------------

PhysicalRegion &PhysicalRegion::operator=(PhysicalRegion &&rhs)
{
    Clear();
    
    m_pageList = rhs.m_pageList;

    rhs.Clear();

    return *this;
}

} // namespace MM

} // namespace BartOS
