#ifndef PHYSICAL_REGION_H
#define PHYSICAL_REGION_H

#include "BartOS.h"

#include "PhysicalPage.h"

namespace BartOS
{

namespace MM
{

//! Forward declarations.
class Pmm;

/*
 *  @brief The contiguous physical memory region.
 */
class PhysicalRegion
{
public:
    /*
     *  @brief Copy constructor.
     * 
     *  @param rhs the physical region to copy.
     */
    PhysicalRegion(const PhysicalRegion &rhs);

    /*
     *  @brief Move constructor.
     * 
     *  @param rhs the physical region to move from.
     */
    PhysicalRegion(PhysicalRegion &&rhs);

    //! Destructor
    ~PhysicalRegion();

    /*
     *  @brief Copy assignment operator.
     * 
     *  @param rhs the physical region to copy.
     */
    PhysicalRegion &operator=(const PhysicalRegion &rhs);

    /*
     *  @brief Move assignment operator.
     * 
     *  @param rhs the physical region to move from.
     */
    PhysicalRegion &operator=(PhysicalRegion &&rhs);

    /*
     *  @brief Check whether the region contains the address.
     * 
     *  @param pAddr the address to check.
     * 
     *  @return whether the region contains the address.
     */
    bool ContainsAddress(const PhysicalAddress pAddr);

    /*
     *  @brief Get the address of the physical region.
     * 
     *  @return The address of the physical region.
     */
    PhysicalAddress GetAddress();

    /*
     *  @brief Get the size of the physical region in bytes.
     * 
     *  @return The size of the physical region in bytes.
     */
    size_t GetSize();
    
    //! Clear the object
    void Clear();

private:
    using PhysicalPagePtrList = Vector<RefPtr<PhysicalPage>>;   ///< Physical page pointer list typedef.

    //! Default constructor. Only constructible by Pmm.
    PhysicalRegion();

    PhysicalPagePtrList     m_physicalPageList;     ///< The physical page pointer list.
    
    friend class Pmm;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

inline PhysicalRegion::PhysicalRegion()
{
}

// ---------------------------------------------------------------------------------------------------------

inline PhysicalRegion::~PhysicalRegion()
{
}

// ---------------------------------------------------------------------------------------------------------

inline bool PhysicalRegion::ContainsAddress(const PhysicalAddress pAddr)
{
    if (m_physicalPageList.empty())
        return false;

    const PhysicalAddress startAddr = m_physicalPageList.front()->GetAddress();
    const PhysicalAddress endAddr = PhysicalAddress(startAddr.Get() + GetSize());

    return ((pAddr >= startAddr) && (pAddr < endAddr));
}

// ---------------------------------------------------------------------------------------------------------

inline PhysicalAddress PhysicalRegion::GetAddress()
{
    if (!m_physicalPageList.empty())
        return m_physicalPageList.front()->GetAddress();

    return PhysicalAddress(0);
}

// ---------------------------------------------------------------------------------------------------------

inline size_t PhysicalRegion::GetSize()
{
    return m_physicalPageList.size() * PhysicalPage::m_pageSize;
}

// ---------------------------------------------------------------------------------------------------------

inline void PhysicalRegion::Clear()
{
    m_physicalPageList.clear();
}

} // namespace MM

} // namespace BartOS

#endif // PHYSICAL_REGION_H