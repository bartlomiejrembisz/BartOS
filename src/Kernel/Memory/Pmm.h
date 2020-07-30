#ifndef PMM_H
#define PMM_h

#include "Kernel/BartOS.h"
#include "Libraries/Misc/Singleton.h"

#include "MemoryPool.h"

namespace BartOS
{

struct multiboot_tag_mmap;

namespace MM
{

//! Forward declare the kernel address space.
class KernelAddressSpace;

//! The Physical Memory Manager singleton.
class Pmm : public Singleton<Pmm>
{
public:
    typedef MemoryPool::PhysicalRange PhysicalRange;    ///< Forward the PhysicalRange type.

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
    const PhysicalRange AllocateRange(const size_t nPages);

    /*
     *  @brief Return a physical page.
     * 
     *  @param pPhysicalPage pointer to the allocated page.
     */
    void ReturnPage(const PhysicalPage * const pPhysicalPage);

    /*
     *  @brief Return a physical page range.
     * 
     *  @param  physicalRange the physical range.
     */
    void ReturnRange(PhysicalRange &physicalRange);

    /*
     *  @brief Get the memory stats.
     * 
     *  @return the memory stats.
     */
    MemoryStats GetMemoryStats();

    /*
     *  @brief Get the end address.
     * 
     *  @return the end address.
     */
    PhysicalAddress GetEndAddress();

private:
    /*
     *  @brief Allocate a physical page range at a specific address.
     *  Used only by the KernelAddressSpace class during init.
     *  
     *  @param  physicalAddress the starting physical address.
     *  @param  nPages the amount of physically contiguous pages.
     * 
     *  @return pointer to the allocated page.
     */
    const PhysicalRange AllocateRange(const PhysicalAddress physicalAddress, const size_t nPages);

    /*
     *  @brief Initialize a physical page range.
     *  
     *  @param  physicalRange the physical range to initialize.
     */
    void InitializePhysicalRange(PhysicalRange &physicalRange);

    MemoryPool  m_memoryPool;       ///< The physical memory pool.
    bool        m_isInitialized;    ///< Whether the object is initialized.

    friend class MemoryPool::PhysicalRange;
    friend class KernelAddressSpace;
    friend class Singleton<Pmm>;
};

} // namespace MM

} // namespace BartOS

#endif // PMM_H
