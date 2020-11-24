#ifndef PHYSICAL_ADDRESS_H
#define PHYSICAL_ADDRESS_H

#include "Address.h"

namespace BartOS
{

class VirtualAddress;

class [[gnu::packed]] PhysicalAddress : public Address<PhysicalAddress>
{
public:
    //! Constructor
    PhysicalAddress();

    /*
     *  @brief Constructor
     * 
     *  @param address the address.
     */
    explicit PhysicalAddress(const Address_t address);

    /*
     *  @brief Create PhysicalAddress from VirtualAddress.
     *         Offsets the address by KERNEL_VMA
     * 
     *  @param virtualAddress the virtual address.
     */
    static PhysicalAddress Create(const VirtualAddress &virtualAddress);

};

} // namespace BartOS

#endif // PHYSICAL_ADDRESS_H
