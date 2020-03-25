#ifndef PHYSICAL_MEMORY_MANAGER_H
#define PHYSICAL_MEMORY_MANAGER_H

#include "Kernel/BartOS.h"

#include "Libraries/Misc/Singleton.h"

namespace BartOS
{

class PhysicalMemoryManager : public Singleton<PhysicalMemoryManager>
{
public:
    static const uint16_t PAGE_SIZE = 4096; //< The physical page size.

    /*
     *  @brief Initialize the physical memory manager.
     *
     *  @return whether initialization is successful.
     */
    bool Initialize();

private:
    //! Constructor
    PhysicalMemoryManager();

    //! Destructor
    ~PhysicalMemoryManager();

    bool    m_isInitialized;

    friend class Singleton<PhysicalMemoryManager>;
};

} // namespace BartOS

#endif // PHYSICAL_MEMORY_MANAGER_H