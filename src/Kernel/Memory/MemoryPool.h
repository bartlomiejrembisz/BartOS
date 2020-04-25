#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "Kernel/BartOS.h"
#include "Libraries/Misc/RefPtr.h"

#include "frg/list.hpp"

#include "PhysicalPage.h"

namespace BartOS
{

namespace MM
{

//! Forward declare the pmm.
class Pmm;

/*
 *  @brief The physical memory pool.
 */
class MemoryPool
{
public:
    /*
     *  @brief The physical memory region.
     */
    class MemoryRegion
    {
    public:
        //! Constructor
        MemoryRegion();

        PhysicalAddress m_addr;     ///< The physical address of the region.
        size_t          m_size;     ///< The size of the region.
    };

private:
    //! The free list typedef.
    using PhysicalPageFreeList = 
    	frg::intrusive_list<
	    	PhysicalPage,
    		frg::locate_member<
			    PhysicalPage,
			    frg::default_list_hook<PhysicalPage>,
			    &PhysicalPage::m_freeListHook
		    >
	    >;

    //! Constructor
    MemoryPool();

    //! Remove copy and move construction and assignment.
    MemoryPool(const MemoryPool &rhs) = delete;
    MemoryPool(MemoryPool &&rhs) = delete;
    MemoryPool &operator=(const MemoryPool &rhs) = delete;
    MemoryPool &operator=(MemoryPool &&rhs) = delete;

    /*
     *  @brief Divide memory region into page sizes and add to pool.
     * 
     *  @param memoryRegion the memory region.
     */
    void AddMemoryRegion(const MemoryRegion &memoryRegion);

    /*
     *  @brief Allocate a physical page.
     * 
     *  @return pointer to the allocated page.
     */
    const PhysicalPage *AllocatePage();

    /*
     *  @brief Allocate a physical page.
     * 
     *  @param pPhysicalPage pointer to the allocated page.
     */
    void ReturnPage(const PhysicalPage * const pPhysicalPage);

    /*
     *  @brief Get a physical page from the pool.
     * 
     *  @param pageAddr the address of the page
     * 
     *  @return pointer to the physical page.
     */
    const RefPtr<PhysicalPage> GetPhysicalPage(const uintptr_t pageAddr);

    PhysicalPage            *m_pPool;       ///< Physical page pool.
    size_t                  m_poolSize;     ///< The size of the pool.
    PhysicalPageFreeList    m_freeList;     ///< The page free list.

    friend class Pmm;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

inline MemoryPool::MemoryRegion::MemoryRegion() :
    m_addr(0),
    m_size(0)
{
}

} // namespace MM

} // namespace BartOS

#endif // MEMORY_POOL_H
