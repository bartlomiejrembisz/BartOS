#include "Address.h"

#include "PhysicalAddress.h"
#include "VirtualAddress.h"

#include "Memory.h"

namespace BartOS
{

template <typename ADDRESS>
Address<ADDRESS>::Address() :
    m_address(0)
{
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address<ADDRESS>::Address(const Address_t address) :
    m_address(address)
{
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::Offset(Address_t offset) const
{
    return ADDRESS(m_address + offset);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address_t Address<ADDRESS>::Get() const
{
    return m_address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
void Address<ADDRESS>::Set(const Address_t address)
{
    m_address = address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::PageAddress(const size_t pageSize) const
{
    return ADDRESS(ALIGN(m_address, pageSize));
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address<ADDRESS>::operator Address_t() const
{
    return m_address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address<ADDRESS>::operator bool() const
{
    return m_address == 0;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator+(const Address_t offset)
{
    return ADDRESS(m_address + offset);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator+(const Address<ADDRESS> &offset)
{
    return ADDRESS(*this + offset.Get());
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS &Address<ADDRESS>::operator+=(const Address_t offset)
{
    m_address += offset;
    return static_cast<ADDRESS &>(*this);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS &Address<ADDRESS>::operator+=(const Address<ADDRESS> &offset)
{
    *this + offset.Get();
    return static_cast<ADDRESS &>(*this);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator-(const Address_t offset)
{
    return ADDRESS(m_address - offset);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator-(const Address<ADDRESS> &offset)
{
    return ADDRESS(*this - offset.Get());
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS &Address<ADDRESS>::operator-=(const Address_t offset)
{
    m_address -= offset;
    return static_cast<ADDRESS &>(*this);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS &Address<ADDRESS>::operator-=(const Address<ADDRESS> &offset)
{
    *this - offset.Get();
    return static_cast<ADDRESS &>(*this);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator&(const Address_t mask)
{
    return ADDRESS(m_address & mask);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS &Address<ADDRESS>::operator&=(const Address_t mask)
{
    *this & mask;
    return static_cast<ADDRESS &>(*this);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator==(const Address<ADDRESS> &rhs) const
{
    return m_address == rhs.m_address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator!=(const Address<ADDRESS> &rhs) const
{
    return m_address != rhs.m_address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator>(const Address<ADDRESS> &rhs) const
{
    return m_address > rhs.m_address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator>=(const Address<ADDRESS> &rhs) const 
{
    return m_address >= rhs.m_address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator<(const Address<ADDRESS> &rhs) const 
{
    return m_address < rhs.m_address;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator<=(const Address<ADDRESS> &rhs) const
{
    return m_address <= rhs.m_address;
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template class Address<PhysicalAddress>;
template class Address<VirtualAddress>;

} // namespace BartOS
