#ifndef PHYSICAL_ADDRESS_H
#define PHYSICAL_ADDRESS_H

#include "Address.h"

namespace BartOS
{

class VirtAddr;

class [[gnu::packed]] PhysAddr : public Address<PhysAddr>
{
public:
    /*
     *  @brief Constructor
     */
    PhysAddr();

    /*
     *  @brief Constructor
     * 
     *  @param address the address.
     */
    PhysAddr(const Address_t address);

    /*
     *  @brief Create PhysAddr from VirtAddr.
     *         Offsets the address by KERNEL_VMA
     * 
     *  @param virtAddr the virtual address.
     */
    static PhysAddr FromVirt(const VirtAddr &virtAddr);
};

} // namespace BartOS

#endif // PHYSICAL_ADDRESS_H
