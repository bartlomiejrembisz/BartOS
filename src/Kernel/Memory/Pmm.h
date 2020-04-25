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
    //! Constructor
    Pmm();

    /*
     *  @brief Initialize the physical 
     */
    StatusCode Initialize(const multiboot_tag_mmap *pMmapTag);

private:
    MemoryPool  m_memoryPool;       ///< The physical memory pool.

    friend class Singleton<Pmm>;
};

} // namespace MM

} // namespace BartOS

#endif // PMM_H
