#ifndef ADDR_RANGE_H
#define ADDR_RANGE_H

#include "BartOS.h"

#include <type_traits>

namespace BartOS
{

/*
 *  @brief An address range
 */
template <typename ADDRESS>
class Range {
public:
    using AddrType = ADDRESS;           ///< The underlying address type.
    using ThisRange = Range<ADDRESS>;   ///< The type of this range.

    /*
     *  @brief Constructor
     * 
     *  @param address the address.
     *  @param size the range size.
     */
    Range(const AddrType &addr = AddrType(0), const size_t size = 0);

    /*
     *  @brief Update the range size.
     * 
     *  @param endAddr the address of the new range end.
     * 
     *  @return whether the object was extended.
     */
    bool SetNewEnd(const AddrType &endAddr);

    /*
     *  @brief Split the range at specified offset.
     * 
     *  @param offset the offset to split at.
     * 
     *  @return range object generated.
     *  The return object will be invalid if split is impossible.
     */
    ThisRange Split(const size_t offset);

    /*
     *  @brief Split the range in the middle.
     * 
     *  @return new range generated.
     *  The return object will be invalid if split is impossible.
     */
    ThisRange Halve();

    /*
     *  @brief Reset the object to make it invalid.
     */
    void Reset();

    /*
     *  @brief Check if the object is valid.
     * 
     *  @return Whether the object is valid.
     */
    bool IsValid() const;

    /*
     *  @brief Get the base address
     * 
     *  @return the base address.
     */
    AddrType GetBase() const;

    /*
     *  @brief Get the end address
     * 
     *  @return the end address.
     */
    AddrType GetEnd() const;

    /*
     *  @brief Get the size
     * 
     *  @return the size of the Range.
     */
    size_t GetSize() const;

    /*
     *  @brief Check the range constains an address range.
     * 
     *  @param addr base of the range.
     *  @param size the size of the range
     * 
     *  @return whether an address is within Range bounds
     */
    bool Contains(const AddrType &addr, const size_t size = 0) const;

    /*
     *  @brief Check the range constains an address range.
     * 
     *  @param range to check.
     * 
     *  @return whether an address is within Range bounds
     */
    bool Contains(const ThisRange &range) const;

protected:
    //! Delete default constructor.
    Range() = delete;

    AddrType    m_addr;
    size_t      m_size;

    static_assert(std::is_same<AddrType, PhysAddr>::value || std::is_same<AddrType, VirtAddr>::value);
};

using PhysRange = Range<PhysAddr>;
using VirtRange = Range<VirtAddr>;

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline Range<ADDRESS>::Range(const AddrType &addr, const size_t size) :
    m_addr(addr),
    m_size(size)
{
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline bool Range<ADDRESS>::SetNewEnd(const AddrType &endAddr)
{
    if (endAddr < m_addr)
        return false;

    m_size = m_addr - endAddr;

    return true;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline auto Range<ADDRESS>::Split(const size_t offset) -> ThisRange
{
    ThisRange newRange;
    if (!Contains(m_addr + offset))
        return newRange;

    const size_t newSize = m_size - offset;

    m_size = offset;

    newRange.m_addr = GetEnd();
    newRange.m_size = newSize;

    return newRange;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline auto Range<ADDRESS>::Halve() -> ThisRange
{
    ThisRange newRange;

    const size_t oldSize = m_size;

    m_size = (oldSize / 2);

    return ThisRange(GetEnd(), (oldSize - m_size));
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline void Range<ADDRESS>::Reset()
{
    m_addr = AddrType(0);
    m_size = 0;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline bool Range<ADDRESS>::IsValid() const
{
    return (0 == m_size);
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline auto Range<ADDRESS>::GetBase() const -> AddrType
{
    return m_addr;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline auto Range<ADDRESS>::GetEnd() const -> AddrType
{
    return m_addr + m_size;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline size_t Range<ADDRESS>::GetSize() const
{
    return m_size;
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline bool Range<ADDRESS>::Contains(const AddrType &addr, const size_t size) const
{
    return (m_addr >= GetBase() && m_addr < GetEnd());
}

// ---------------------------------------------------------------------------------------------------------

template <typename ADDRESS>
inline bool Range<ADDRESS>::Contains(const ThisRange &range) const
{
    return Contains(range.m_addr, range.m_size);
}

} // namespace BartOS

#endif // ADDR_RANGE_H
