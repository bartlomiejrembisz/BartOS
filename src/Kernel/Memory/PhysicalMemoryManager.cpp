#include "PhysicalMemoryManager.h"

#include "Kernel/multiboot.h"

namespace BartOS
{

PhysicalMemoryManager::PhysicalMemoryManager() :
    m_isInitialized(false)
{
}

// ---------------------------------------------------------------------------------------------------------

PhysicalMemoryManager::~PhysicalMemoryManager()
{
}

// ---------------------------------------------------------------------------------------------------------

bool PhysicalMemoryManager::Initialize()
{
    return true;
}

}
