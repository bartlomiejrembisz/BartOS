#ifndef ADDRESS_H
#define ADDRESS_H

#include "CoreDefs.h"

namespace BartOS
{

template <typename ADDRESS>
class Address {
public:
    /*
     *  @brief Offset the address
     * 
     *  @param offset the offset of the address.
     */
    ADDRESS Offset(const Address_t offset) const;

    /*
     *  @brief Get the address as Address_t
     * 
     *  @return the address as Address_t.
     */
    Address_t Get() const;

    /*
     *  @brief Set the address as Address_t
     * 
     *  @param address the address as Address_t.
     */
    void Set(const Address_t addr);

    /*
     *  @brief Get the page offset for the address.
     * 
     *  @param pageSize the size of the page.
     */
    ADDRESS PageAddr(const size_t pageSize) const;

    //! operator Address_t for conversion.
    operator Address_t() const;

    //! operator bool for bool conversion
    operator bool() const;

    //! operator POINTER for pointer conversion.
    template <typename POINTER>
    operator POINTER*() const;

    //! Arithmetic operators.
    ADDRESS operator+(const Address_t offset) const;
    ADDRESS operator+(const Address<ADDRESS> &offset) const;
    ADDRESS &operator+=(const Address_t offset);
    ADDRESS &operator+=(const Address<ADDRESS> &offset);

    ADDRESS operator-(const Address_t offset) const;
    ADDRESS operator-(const Address<ADDRESS> &offset) const;
    ADDRESS &operator-=(const Address_t offset);
    ADDRESS &operator-=(const Address<ADDRESS> &offset);

    /*
     *  @brief operator & for masking
     * 
     *  @param mask the mask
     */
    ADDRESS operator&(const Address_t mask) const;
    ADDRESS &operator&=(const Address_t mask);

    //! Logical operators.
    bool operator==(const Address<ADDRESS> &rhs) const;
    bool operator!=(const Address<ADDRESS> &rhs) const;
    bool operator>(const Address<ADDRESS> &rhs) const;
    bool operator>=(const Address<ADDRESS> &rhs) const;
    bool operator<(const Address<ADDRESS> &rhs) const;
    bool operator<=(const Address<ADDRESS> &rhs) const;

protected:
    //! Constructor
    Address();

    /*
     *  @brief Constructor
     * 
     *  @param address the address.
     */
    Address(const Address_t addr);

    /*
     *  @brief Copy constructor
     * 
     *  @param rhs the object to copy from.
     */
    Address(const ADDRESS &addr);

    Address_t m_addr;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
template <typename POINTER>
Address<ADDRESS>::operator POINTER*() const
{
    return reinterpret_cast<POINTER*>(m_addr);
}

} // namespace BartOS

#endif // ADDRESS_H
