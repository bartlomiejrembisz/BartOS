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

class Pmm : public Singleton<Pmm>
{
public:
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
     *  @brief Return a physical page.
     * 
     *  @param pPhysicalPage pointer to the allocated page.
     */
    void ReturnPage(const PhysicalPage * const pPhysicalPage);

    /*
     *  @brief Get the memory stats.
     * 
     *  @return the memory stats.
     */
    MemoryStats GetMemoryStats();

private:
    MemoryPool  m_memoryPool;       ///< The physical memory pool.

    friend class Singleton<Pmm>;
};

} // namespace MM

} // namespace BartOS

#endif // PMM_H
