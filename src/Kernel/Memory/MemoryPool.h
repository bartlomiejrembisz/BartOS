#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "Kernel/BartOS.h"
#include "Libraries/Misc/RefPtr.h"
#include "Libraries/Misc/Range.h"

#include "frg/list.hpp"

#include "PhysicalPage.h"

namespace BartOS
{

namespace MM
{

//! Forward declare the pmm and vmm.
class Pmm;
class Vmm;

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

    /*
     *  @brief The physical memory range.
     */
    class PhysicalRange
    {
    public:
        //! Constructor
        PhysicalRange();

        /*
         *  @brief Copy constructor.
         * 
         *  @param rhs the physical range to copy.
         */
        PhysicalRange(const PhysicalRange &rhs);

        /*
         *  @brief Move constructor.
         * 
         *  @param rhs the physical range to move from.
         */
        PhysicalRange(PhysicalRange &&rhs);
    
        //! Destructor
        ~PhysicalRange();

        /*
         *  @brief Copy assignment operator.
         * 
         *  @param rhs the physical range to copy.
         */
        PhysicalRange &operator=(const PhysicalRange &rhs);

        /*
         *  @brief Move assignment operator.
         * 
         *  @param rhs the physical range to move from.
         */
        PhysicalRange &operator=(PhysicalRange &&rhs);

        //! Return the physical range to the memory pool.
        void ReturnToPool();

        /*
         *  @brief Get the address of the physical range.
         * 
         *  @return The address of the physical range.
         */
        PhysicalAddress GetAddress();

        /*
         *  @brief Get the size of the physical range.
         * 
         *  @return The size of the physical range.
         */
        size_t GetSize();
        
        //! Clear the object
        void Clear();

        //! Is the object initialized.
        bool IsInitalized();

    private:
        /*
         *  @brief Constructor.
         * 
         *  @param pPhysicalPage pointer to the first page.
         *  @param nPages the number of pages.
         */
        PhysicalRange(PhysicalPage * const pPhysicalPage, const size_t nPages);

        const PhysicalPage    *m_pPhysicalPage;
        size_t                m_nPages;
        
        friend MemoryPool;
        friend Vmm;
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

    //! Initialize the memory pool.
    void Initialize();

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
     *  @brief Allocate a physical page range.
     * 
     *  @param  nPages the amount of physically contiguous pages.
     * 
     *  @return pointer to the allocated page.
     */
    PhysicalRange AllocateRange(const size_t nPages);

    /*
     *  @brief Allocate a physical page range at a specific address.
     *  Used only by the KernelAddressSpace class during init.
     *  
     *  @param  physicalAddress the starting physical address.
     *  @param  nPages the amount of physically contiguous pages.
     * 
     *  @return pointer to the allocated page.
     */
    PhysicalRange AllocateRange(const PhysicalAddress physicalAddress, const size_t nPages);

    /*
     *  @brief Initialize a physical page range.
     *  
     *  @param  physicalRange the physical range to initialize.
     */
    void InitializePhysicalRange(PhysicalRange &physicalRange);

    /*
     *  @brief Return a physical page.
     * 
     *  @param pPhysicalPage pointer to the allocated page.
     */
    void ReturnPage(const PhysicalPage * const pPhysicalPage);

    /*
     *  @brief Return a physical page.
     * 
     *  @param pageAddress the address of the page.
     */
    void ReturnPage(const PhysicalAddress pageAddress);

    /*
     *  @brief Return a physical page range.
     * 
     *  @param  physicalRange pointer to the physical range.
     */
    void ReturnRange(MemoryPool::PhysicalRange &physicalRange);

    /*
     *  @brief Find a physical page from the pool.
     *  CAUTION: This function does not increment the ref counter.
     * 
     *  @param pageAddr the address of the page
     * 
     *  @return pointer to the physical page.
     */
    const PhysicalPage *FindPhysicalPage(const PhysicalAddress pageAddress);
    
    /*
     *  @brief Get a physical page from the pool.
     * 
     *  @param pageAddr the address of the page
     * 
     *  @return pointer to the physical page.
     */
    const RefPtr<PhysicalPage> GetPhysicalPage(const PhysicalAddress pageAddress);

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

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

inline MemoryPool::PhysicalRange::PhysicalRange() :
    m_pPhysicalPage(nullptr),
    m_nPages(0)
{
}

// ---------------------------------------------------------------------------------------------------------

inline MemoryPool::PhysicalRange::~PhysicalRange()
{
    ReturnToPool();
}

// ---------------------------------------------------------------------------------------------------------

inline PhysicalAddress MemoryPool::PhysicalRange::GetAddress()
{
    return m_pPhysicalPage->GetAddress();
}

// ---------------------------------------------------------------------------------------------------------

inline size_t MemoryPool::PhysicalRange::GetSize()
{
    return m_nPages * PAGE_SIZE;
}

// ---------------------------------------------------------------------------------------------------------

inline void MemoryPool::PhysicalRange::Clear()
{
    m_pPhysicalPage = nullptr;
    m_nPages = 0;
}

// ---------------------------------------------------------------------------------------------------------

inline bool MemoryPool::PhysicalRange::IsInitalized()
{
    return m_pPhysicalPage && (m_nPages != 0);
}

} // namespace MM

} // namespace BartOS

#endif // MEMORY_POOL_H
