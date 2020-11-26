#ifndef ADDRESS_SPACE_H
#define ADDRESS_SPACE_H

#include "Kernel/BartOS.h"
#include "Libraries/Misc/RefPtr.h"

#include "Kernel/Memory/Paging/PageTable.h"

#include "VMArea.h"

namespace BartOS
{

namespace MM
{ 
    
// Forward declare the Vmm
class Vmm;

/*
 *  @brief The address space abstract class.
 */
class AddressSpace
{
public:
    /*
     *  @brief Constructor.
     * 
     *  Can only be used after the memory manager has been initialized.
     *  Will allocate a page alligned kernel page for the PageTable.
     */
    AddressSpace();

    /*
     *  @brief Constructor.
     * 
     *  Can be used before the memory manager has been initialized.
     * 
     *  @param  pPageTable the pointer to the page table
     */
    AddressSpace(MM::PageTable * const pPageTable);

    //! Destructor.
    virtual ~AddressSpace();

    /*
     *  @brief Allocate memory for this address space.
     * 
     *  @param  nBytes ammount to allocate.
     *  @param  pageSize the page size.
     *  @param  pageFlags the page flags
     */
    virtual void *Allocate(size_t nBytes, const PageSize pageSize, const PageFlags pageFlags = NO_FLAGS) = 0;

    /*
     *  @brief Get the address space break.
     * 
     *  @return address space break.
     */
    Address_t GetAddressSpaceBreak();

protected:

    MM::PageTable   *m_pPageTable;       ///< Pointer to the P4 Page Table object.
    Address_t       m_addressBreak;     ///< Where does the address break.

    friend class VMArea;
    friend class MM::Vmm;
};

// ---------------------------------------------------------------------------------------------------------

inline Address_t AddressSpace::GetAddressSpaceBreak()
{
    return m_addressBreak;
}

} // namespace MM

} // namespace BartOS

#endif // ADDRESS_SPACE_H
