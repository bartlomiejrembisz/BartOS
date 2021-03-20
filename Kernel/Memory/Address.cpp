#include "Address.h"

#include "PhysAddr.h"
#include "VirtAddr.h"

#include "Memory.h"

namespace BartOS
{

template <typename ADDRESS>
Address<ADDRESS>::Address() :
    m_addr(0)
{
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address<ADDRESS>::Address(const Address_t addr) :
    m_addr(addr)
{
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address<ADDRESS>::Address(const ADDRESS &addr) :
    m_addr(addr.m_addr)
{
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::Offset(Address_t offset) const
{
    return ADDRESS(m_addr + offset);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address_t Address<ADDRESS>::Get() const
{
    return m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
void Address<ADDRESS>::Set(const Address_t addr)
{
    m_addr = addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::PageAddr(const size_t pageSize) const
{
    return ADDRESS(ALIGN(m_addr, pageSize));
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address<ADDRESS>::operator Address_t() const
{
    return m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
Address<ADDRESS>::operator bool() const
{
    return m_addr == 0;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator+(const Address_t offset) const
{
    return ADDRESS(m_addr + offset);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator+(const Address<ADDRESS> &offset) const
{
    return ADDRESS(*this + offset.Get());
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS &Address<ADDRESS>::operator+=(const Address_t offset)
{
    m_addr += offset;
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
ADDRESS Address<ADDRESS>::operator-(const Address_t offset) const
{
    return ADDRESS(m_addr - offset);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS Address<ADDRESS>::operator-(const Address<ADDRESS> &offset) const
{
    return ADDRESS(*this - offset.Get());
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
ADDRESS &Address<ADDRESS>::operator-=(const Address_t offset)
{
    m_addr -= offset;
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
ADDRESS Address<ADDRESS>::operator&(const Address_t mask) const
{
    return ADDRESS(m_addr & mask);
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
    return m_addr == rhs.m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator!=(const Address<ADDRESS> &rhs) const
{
    return m_addr != rhs.m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator>(const Address<ADDRESS> &rhs) const
{
    return m_addr > rhs.m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator>=(const Address<ADDRESS> &rhs) const 
{
    return m_addr >= rhs.m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator<(const Address<ADDRESS> &rhs) const 
{
    return m_addr < rhs.m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
bool Address<ADDRESS>::operator<=(const Address<ADDRESS> &rhs) const
{
    return m_addr <= rhs.m_addr;
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template class Address<PhysAddr>;
template class Address<VirtAddr>;

} // namespace BartOS
