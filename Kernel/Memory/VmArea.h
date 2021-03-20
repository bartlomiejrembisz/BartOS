#ifndef VM_AREA_H
#define VM_AREA_H

#include "BartOS.h"

#include "Paging/Paging.h"
#include "PhysicalRegion.h"

#include "VM.h"

namespace BartOS
{

// Forward declaration.
namespace x86_64 { namespace Interrupt { class PageFaultHandler; } }

namespace MM
{

// Forward declaration.
class AddressSpace;
class Vmm;

/*
 *  @brief The Virtual Memory Area class.
 */
class VmArea
{
public:
    /*
     *  @brief Set page flags.
     * 
     *  @param pageFlags the page flags for VM area to set.
     */
    void SetPageFlags(const PageFlags pageFlags);

    /*
     *  @brief Unset page flags.
     * 
     *  @param pageFlags the page flags for VM area to unset.
     */
    void UnsetPageFlags(const PageFlags pageFlags);

    /*
     *  @brief Set VM area flags.
     * 
     *  @param VmAreaFlags the flags for VM area to set.
     */
    void SetPageFlags(const VmAreaFlags vmAreaFlags);

    /*
     *  @brief Unset VM area flags.
     * 
     *  @param vmAreaFlags the flags for VM area to unset.
     */
    void UnsetPageFlags(const VmAreaFlags vmAreaFlags);

    /*
     *  @brief Get the start address of the VmArea.
     * 
     *  @return the start address of the VmArea.
     */
    VirtAddr GetStartAddr() const;

    /*
     *  @brief Get the end address of the VmArea.
     * 
     *  @return the end address of the VmArea.
     */
    VirtAddr GetEndAddr() const;

    /*
     *  @brief Get the page flags.
     * 
     *  @return the page flags.
     */
    PageFlags GetPageFlags() const;

    /*
     *  @brief Get the vm area flags.
     * 
     *  @return the vm area flags.
     */
    VmAreaFlags GetFlags() const;

    /*
     *  @brief Is the VmArea allocated.
     * 
     *  @return whether the AMArea is allocated.
     */
    bool IsAllocated() const;

private:
    /*
     *  @brief Construct the object.
     *  Only the AddressSpace can create a VmArea.
     * 
     *  @param vstart the start of the vm area.
     *  @param vend the end of the vm area.
     *  @param addressSpace the address space owning this VmArea.
     */
    VmArea(const VmAreaType vmAreaType, const VirtAddr vstart, const VirtAddr vend, AddressSpace &addressSpace);

    //! Destructor
    ~VmArea();

    //! Disable default, copy and move construction.
    VmArea() = delete;
    VmArea(const VmArea &rhs) = delete;
    VmArea &operator=(const VmArea &rhs) = delete;
    VmArea(VmArea &&rhs) = delete;
    VmArea &operator=(VmArea &&rhs) = delete;

    VmAreaType                  m_vmAreaType;           ///< The type of the vm area.

    VirtAddr                    m_vstart;               ///< The start address of the area.
    VirtAddr                    m_vend;                 ///< The end address of the area.
    AddressSpace                &m_addressSpace;        ///< The Address Space reference.

    PageFlags                   m_pageFlags;            ///< The page flags.
    VmAreaFlags                 m_vmAreaFlags;          ///< The type of VM area.
    PageSize                    m_pageSize;             ///< The page size.

    friend class Vmm;
    friend class PageFaultHandler;
    friend class AddressSpace;
};

// ---------------------------------------------------------------------------------------------------------

inline VirtAddr VmArea::GetStartAddr() const
{
    return m_vstart;
}

// ---------------------------------------------------------------------------------------------------------

inline VirtAddr VmArea::GetEndAddr() const
{
    return m_vend;
}

// ---------------------------------------------------------------------------------------------------------

inline PageFlags VmArea::GetPageFlags() const
{
    return m_pageFlags;
}

// ---------------------------------------------------------------------------------------------------------

inline VmAreaFlags VmArea::GetFlags() const
{
    return m_vmAreaFlags;
}

// ---------------------------------------------------------------------------------------------------------

inline bool VmArea::IsAllocated() const
{
    return ((m_vstart.Get() == 0) && (m_vend.Get() == 0));
}

} // name MM

} // namespace BartOS

#endif // VM_AREA_H
