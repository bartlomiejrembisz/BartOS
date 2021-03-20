#include "PhysAddr.h"

#include "VirtAddr.h"

#include "Memory.h"

namespace BartOS
{

PhysAddr::PhysAddr()
{
}

// ---------------------------------------------------------------------------------------------------------

PhysAddr::PhysAddr(const Address_t address) : 
    Address<PhysAddr>(address)
{
}

// ---------------------------------------------------------------------------------------------------------

PhysAddr PhysAddr::FromVirt(const VirtAddr &virtAddr)
{
    return PhysAddr(virtAddr.Get() - reinterpret_cast<Address_t>(KERNEL_VMA));
}

} // namespace BartOS
