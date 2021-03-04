#ifndef PMM_H
#define PMM_h

#include "BartOS.h"

#include "PhysicalPage.h"
#include "PhysicalRegion.h"

#include "Libraries/Misc/Singleton.h"

namespace BartOS
{

struct multiboot_tag_mmap;

namespace MM
{

//! Forward declarations.
class AddressSpace;
class Vmm;

//! The Physical Memory Manager singleton.
class Pmm : public Singleton<Pmm>
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
     *  @brief The memory stats.
     */
    struct MemoryStats
    {
    public:
        size_t m_totalMemory;
        size_t m_usedMemory;
    };

    //! Constructor
    Pmm();

    /*
     *  @brief Initialize the physical 
     */
    StatusCode Initialize(const multiboot_tag_mmap *pMmapTag);

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
    RefPtr<PhysicalPage> AllocatePage();

    /*
     *  @brief Allocate a contiguous physical page region.
     * 
     *  @param  nPages the amount of physically contiguous pages.
     * 
     *  @return the physical region object.
     */
    PhysicalRegion AllocateRegion(const size_t nPages);

    /*
     *  @brief Allocate a physical page region at a specific address.
     *  Used only by the AddressSpace class during init.
     *  
     *  @param  physicalAddress the starting physical address.
     *  @param  nPages the amount of physically contiguous pages.
     * 
     *  @return pointer to the allocated page.
     */
    const PhysicalRegion AllocateRegion(const PhysicalAddress physicalAddress, const size_t nPages);

    /*
     *  @brief Return a physical page.
     * 
     *  @param pPhysicalPage pointer to the allocated page.
     */
    void ReturnPage(PhysicalPage * const pPhysicalPage);

    /*
     *  @brief Get a physical page from the pool.
     * 
     *  @param pageAddr the address of the page
     * 
     *  @return pointer to the physical page.
     */
    RefPtr<PhysicalPage> GetPhysicalPage(const PhysicalAddress pageAddress);

    /*
     *  @brief Get the memory stats.
     * 
     *  @return the memory stats.
     */
    MemoryStats GetMemoryStats();

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

    bool                    m_isInitialized;    ///< Whether the object is initialized.

    PhysicalPage            *m_pPool;           ///< Physical page pool.
    size_t                  m_poolSize;         ///< The size of the pool.
    PhysicalPageFreeList    m_freeList;         ///< The page free list.
    size_t                  m_freeListSize;     ///< The size of the free list.

    friend class Singleton<Pmm>;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

inline Pmm::MemoryRegion::MemoryRegion() :
    m_addr(0),
    m_size(0)
{
}

} // namespace MM

} // namespace BartOS

#endif // PMM_H
