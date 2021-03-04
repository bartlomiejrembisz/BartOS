#ifndef PHYSICAL_PAGE_H
#define PHYSICAL_PAGE_H

#include "BartOS.h"
#include "Libraries/Misc/RefPtr.h"

#include "frg/list.hpp"

namespace BartOS
{

namespace MM
{

class Pmm;

class PhysicalPage : public RefCounter<PhysicalPage>
{
public:
    typedef RefCounter<PhysicalPage> Parent;                ///< The ref counter parent typedef.

    static constexpr uint16_t m_pageSize = PAGE_SIZE;   ///< The page size.

    /*
     *  @brief Get the physical address of the page
     * 
     *  @return the physical address of the page.
     */
    PhysicalAddress GetAddress() const;

    frg::default_list_hook<PhysicalPage> m_freeListHook;    ///< frg intrusive list interface.

private:
    //! Disable default and copy construction.
    PhysicalPage() = delete;
    PhysicalPage(const PhysicalPage &rhs) = delete;
    PhysicalPage &operator=(const PhysicalPage &rhs) = delete;

    /*
     *  @brief Constructor
     *  Only the Pmm can create a physical page.
     * 
     *  @param paddr the physical address.
     */
    PhysicalPage(const PhysicalAddress paddr);

    /*
     *  @brief Move constructor
     *  Only the Pmm can move a physical page.
     *  Used only during physical page creation.
     * 
     *  @param rhs the page to move from.
     */
    PhysicalPage(PhysicalPage &&rhs);

    /*
     *  @brief Move assignment operator
     *  Only the Pmm can move a physical page.
     *  Used only during physical page creation.
     * 
     *  @param rhs the page to move from.
     * 
     *  @return reference to this object.
     */
    PhysicalPage &operator=(PhysicalPage &&rhs);

    //! RefCounter interface
    static void OnDie(PhysicalPage &physicalPage);

    PhysicalAddress m_addr;     ///< The physical address of the page.

    friend class Pmm;
    friend class RefCounter<PhysicalPage>;
};

} // namespace MM

} // namespace BartOS


#endif // PHYSICAL_PAGE_H
