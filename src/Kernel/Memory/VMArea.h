#ifndef VM_AREA_H
#define VM_AREA_H

#include "Kernel/BartOS.h"
#include "Kernel/Memory/Paging/PageTableEntry.h"

#include "MemoryPool.h"

namespace BartOS
{

// Forward declaration.
inline namespace x86_64 { namespace Interrupt { class PageFaultHandler; } }

namespace MM
{

// Forward declaration.
class AddressSpace;
class KernelAddressSpace;
class Vmm;
class PhysicalPage;

/*
 *  @brief The Virtual Memory Area class.
 */
class VMArea
{
public:
    enum VMAreaType
    {
        UNDEFINED,
        PERMANENT
    };

    /*
     *  @brief Get the start address of the VMArea.
     * 
     *  @return the start address of the VMArea.
     */
    VirtualAddress GetStartAddress() const;

    /*
     *  @brief Get the end address of the VMArea.
     * 
     *  @return the end address of the VMArea.
     */
    VirtualAddress GetEndAddress() const;

    /*
     *  @brief Get the flags.
     * 
     *  @return the page flags.
     */
    PageFlags GetFlags() const;

    /*
     *  @brief Is the VMArea allocated.
     * 
     *  @return whether the AMArea is allocated.
     */
    bool IsAllocated() const;

private:
    //! Constructor
    VMArea();

    //! Destructor
    ~VMArea();

    //! Disable default and copy construction.
    VMArea(const VMArea &rhs) = delete;
    VMArea &operator=(const VMArea &rhs) = delete;

    /*
     *  @brief Initialize the object.
     *  Only the AddressSpace can create a VMArea.
     * 
     *  @param vstart the start of the vm area.
     *  @param vend the end of the vm area.
     *  @param addressSpace the address space owning this VMArea.
     */
    void Initialize(const VirtualAddress vstart, const VirtualAddress vend, AddressSpace &addressSpace, const PageFlags pageFlags);

    /*
     *  @brief Set VM area flags.
     * 
     *  @param pageFlags the page flags for VM area to set.
     */
    void SetFlag(const PageFlags pageFlags);

    /*
     *  @brief Unset VM area flags.
     * 
     *  @param pageFlags the page flags for VM area to unset.
     */
    void UnsetFlag(const PageFlags pageFlags);

    VirtualAddress              m_vstart;               ///< The start address of the area.
    VirtualAddress              m_vend;                 ///< The end address of the area.
    AddressSpace                *m_pAddressSpace;       ///< The Address Space pointer.
    MemoryPool::PhysicalRange   m_physicalRange;        ///< The physical range.
    PageFlags                   m_flags;                ///< The page flags.
    VMAreaType                  m_vmAreaType;           ///< The type of VM area.

    friend class Vmm;
    friend class Interrupt::PageFaultHandler;
    friend class AddressSpace;
    friend class KernelAddressSpace;
};

// ---------------------------------------------------------------------------------------------------------

inline VirtualAddress VMArea::GetStartAddress() const
{
    return m_vstart;
}

// ---------------------------------------------------------------------------------------------------------

inline VirtualAddress VMArea::GetEndAddress() const
{
    return m_vend;
}

// ---------------------------------------------------------------------------------------------------------

inline PageFlags VMArea::GetFlags() const
{
    return m_flags;
}

// ---------------------------------------------------------------------------------------------------------

inline bool VMArea::IsAllocated() const
{
    return (m_pAddressSpace);
}

} // name MM

} // namespace BartOS

#endif // VM_AREA_H
