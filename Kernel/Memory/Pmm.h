#ifndef PMM_H
#define PMM_h

#include "BartOS.h"

#include "Page.h"
#include "PhysicalRegion.h"
#include "Range.h"

#include "Lib/Core/Singleton.h"

namespace BartOS
{

struct multiboot_tag_mmap;
struct multiboot_mmap_entry;

namespace MM
{

//! Forward declarations.
class AddressSpace;
class Vmm;

//! The Physical Memory Manager singleton.
class Pmm : public Singleton<Pmm>
{
public:
    static PhysRange    m_reservedKernelRange;  ///< Reserved kernel memory.

    /*
     *  @brief The memory stats.
     */
    struct MemoryStats
    {
    public:
        size_t m_totalMemory;
        size_t m_usedMemory;
    };

    /*
     *  @brief Initialize the physical 
     */
    StatusCode Initialize(const multiboot_tag_mmap *pMmapTag);

    /*
     *  @brief Create a Zone managing the memory chunk.
     * 
     *  @param pMemoryEntry object describing an available memory block.
     */
    void CreateZone(const multiboot_mmap_entry *pMemoryEntry);

    /*
     *  @brief Allocate a physical page.
     * 
     *  @return pointer to the allocated page.
     */
    RefPtr<Page> AllocatePage();

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
     *  @param  physAddr the starting physical address.
     *  @param  nPages the amount of physically contiguous pages.
     * 
     *  @return pointer to the allocated page.
     */
    const PhysicalRegion AllocateRegion(const PhysAddr physAddr, const size_t nPages);

    /*
     *  @brief Return a physical page.
     * 
     *  @param pPage pointer to the allocated page.
     */
    void ReturnPage(Page * const pPage);

    /*
     *  @brief Get a physical page from the pool.
     * 
     *  @param pageAddr the address of the page
     * 
     *  @return pointer to the physical page.
     */
    RefPtr<Page> GetPage(const PhysAddr pageAddress);

    /*
     *  @brief Get the memory stats.
     * 
     *  @return the memory stats.
     */
    MemoryStats GetMemoryStats();

private:
    //! Constructor
    Pmm();


    friend class Singleton<Pmm>;
};

} // namespace MM

} // namespace BartOS

#endif // PMM_H
