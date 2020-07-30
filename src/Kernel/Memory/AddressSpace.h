#ifndef ADDRESS_SPACE_H
#define ADDRESS_SPACE_H

#include "Kernel/BartOS.h"
#include "Libraries/Misc/RefPtr.h"

#include "Kernel/Memory/Paging/PageTable.h"

#include "VirtualPage.h"

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
     *  @brief Retrieve a Virtual Page object.
     * 
     *  @param virtualAddress the virtual address of the page.
     */
    virtual VirtualPage *GetVirtualPage(const VirtualAddress virtualAddress) = 0;

protected:
    MM::PageTable *m_pPageTable;       ///< Pointer to the P4 Page Table object.

    friend class VirtualPage;
    friend class MM::Vmm;
};

} // namespace MM

} // namespace BartOS

#endif // ADDRESS_SPACE_H
