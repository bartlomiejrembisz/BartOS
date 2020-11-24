#include "PhysicalAddress.h"

#include "VirtualAddress.h"

#include "Memory.h"

namespace BartOS
{

PhysicalAddress::PhysicalAddress()
{
}

// ---------------------------------------------------------------------------------------------------------

PhysicalAddress::PhysicalAddress(const Address_t address) : 
    Address<PhysicalAddress>(address)
{
}

// ---------------------------------------------------------------------------------------------------------

PhysicalAddress PhysicalAddress::Create(const VirtualAddress &virtualAddress)
{
    return PhysicalAddress(virtualAddress.Get() - reinterpret_cast<Address_t>(KERNEL_VMA));
}

} // namespace BartOS
