#ifndef PHYSICAL_PAGE_H
#define PHYSICAL_PAGE_H

#include "Kernel/BartOS.h"
#include "Libraries/Misc/RefCounter.h"

#include "frg/list.hpp"

namespace BartOS
{

namespace MM
{

class Pmm;
class MemoryPool;

class PhysicalPage : public RefCounter<PhysicalPage>
{
public:
    typedef RefCounter<PhysicalPage> Parent;    ///< The ref counter parent typedef.

    /*
     *  @brief Get the physical address of the page
     * 
     *  @return the physical address of the page.
     */
    PhysicalAddress GetAddress() const;

    /*
     *  @brief Get the size of the page
     * 
     *  @return the size of the page.
     */
    static constexpr uint16_t GetSize();

    //! RefCounter interface
    static void OnDie(const Parent &object);

    frg::default_list_hook<PhysicalPage> m_freeListHook;    ///< frg intrusive list interface.

private:
    static constexpr uint16_t m_pageSize = PAGE_SIZE;   ///< The page size.

    //! Disable default and copy construction.
    PhysicalPage() = delete;
    PhysicalPage(const PhysicalPage &rhs) = delete;
    PhysicalPage &operator=(const PhysicalPage &rhs) = delete;

    /*
     *  @brief Constructor
     *  Only the MemoryPool can create a physical page.
     * 
     *  @param paddr the physical address.
     */
    PhysicalPage(const PhysicalAddress paddr);

    /*
     *  @brief Move constructor
     *  Only the MemoryPool can move a physical page.
     *  Used only during physical page creation.
     * 
     *  @param rhs the page to move from.
     */
    PhysicalPage(PhysicalPage &&rhs);

    /*
     *  @brief Move assignment operator
     *  Only the MemoryPool can move a physical page.
     *  Used only during physical page creation.
     * 
     *  @param rhs the page to move from.
     * 
     *  @return reference to this object.
     */
    PhysicalPage &operator=(PhysicalPage &&rhs);

    PhysicalAddress m_addr;     ///< The physical address of the page.

    friend class MemoryPool;
    friend class RefCounter<PhysicalPage>;
};

} // namespace MM

} // namespace BartOS


#endif // PHYSICAL_PAGE_H
