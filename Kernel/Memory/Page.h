#ifndef PHYSICAL_PAGE_H
#define PHYSICAL_PAGE_H

#include "BartOS.h"
#include "Lib/Core/RefPtr.h"

namespace BartOS
{

namespace MM
{

class Pmm;

class Page : public RefCounter<Page>
{
public:
    typedef RefCounter<Page> Parent;                ///< The ref counter parent typedef.

    static constexpr uint16_t m_pageSize = PAGE_SIZE;   ///< The page size.

    /*
     *  @brief Get the physical address of the page
     * 
     *  @return the physical address of the page.
     */
    PhysAddr GetAddress() const;

private:
    //! Disable default and copy construction.
    Page() = delete;
    Page(const Page &rhs) = delete;
    Page &operator=(const Page &rhs) = delete;

    /*
     *  @brief Constructor
     *  Only the Pmm can create a physical page.
     * 
     *  @param paddr the physical address.
     */
    Page(const PhysAddr paddr);

    /*
     *  @brief Move constructor
     *  Only the Pmm can move a physical page.
     *  Used only during physical page creation.
     * 
     *  @param rhs the page to move from.
     */
    Page(Page &&rhs);

    /*
     *  @brief Move assignment operator
     *  Only the Pmm can move a physical page.
     *  Used only during physical page creation.
     * 
     *  @param rhs the page to move from.
     * 
     *  @return reference to this object.
     */
    Page &operator=(Page &&rhs);

    //! RefCounter interface
    static void OnDie(Page &page);

    PhysAddr m_addr;     ///< The physical address of the page.

    friend class Pmm;
    friend class RefCounter<Page>;
};

} // namespace MM

} // namespace BartOS


#endif // PHYSICAL_PAGE_H
