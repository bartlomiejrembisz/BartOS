#include "PhysicalRegion.h"

namespace BartOS
{

namespace MM
{

PhysicalRegion::PhysicalRegion(const PhysicalRegion &rhs) :
    m_physicalPageList(rhs.m_physicalPageList)
{
}

// ---------------------------------------------------------------------------------------------------------

PhysicalRegion::PhysicalRegion(PhysicalRegion &&rhs) :
    m_physicalPageList(rhs.m_physicalPageList)
{
    rhs.Clear();
}

// ---------------------------------------------------------------------------------------------------------

PhysicalRegion &PhysicalRegion::operator=(const PhysicalRegion &rhs)
{
    Clear();
    
    m_physicalPageList = rhs.m_physicalPageList;

    return *this;
}

// ---------------------------------------------------------------------------------------------------------

PhysicalRegion &PhysicalRegion::operator=(PhysicalRegion &&rhs)
{
    Clear();
    
    m_physicalPageList = rhs.m_physicalPageList;

    rhs.Clear();

    return *this;
}

} // namespace MM

} // namespace BartOS
