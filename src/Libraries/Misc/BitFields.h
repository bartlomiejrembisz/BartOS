#ifndef BITFIELDS_H
#define BITFIELDS_H

#include "CoreDefs.h"

#include "frg/macros.hpp"
#include "cxxshim/type_traits"

namespace BartOS
{

/*
 * Usage of this template is the following:
 * 
 * class MyBitmap : Bitmap<8>
 * {
 *  typedef BitField<MyBitmap, 1> OneBit;
 *  typedef BitField<OneBit, 2> TwoBits;
 *  typedef BitField<TwoBit, 4> FourBits;
 *  typedef BitField<FourBits, 1> OneBit;
 * };
 * 
 * MyBitmap myBitmap // The bitmap object, default initialized to 0.
 * 
 * myBitmap.Set<MyBitmap::OneBit>(1);   // Set the first bit to 1.
 * myBitmap.Set<MyBitmap::TwoBit>(3);   // Set the third and fourth bits to 3.
 * myBitmap.Set<MyBitmap::FourBits>(15);   // Set the fith, sixth, seventh and eighth bits to 15.
 */

/*
 *  @brief Bitmap class 
 */
template <uint64_t N>
class Bitmap
{
public:
    typedef typename BitTraits<N>::IntegralType Type;   //< The underlying type.

    //! Constructor
    Bitmap() : m_value(0)
    {
    }

    /*
     *  @brief Constructor.
     * 
     *  @param value the default init value.
     */
    Bitmap(const Type value) : m_value(value)
    {
    }

    /*
     *  @brief Set the underlying value.
     */
    void Set(const Type value)
    {
        m_value = value;
    }

    /*
     *  @brief Set the bit field.
     */
    template<typename BIT_FIELD>
    void Set(const typename BIT_FIELD::ValueType value)
    {
        FRG_ASSERT(value <= BIT_FIELD::m_maximum);

        BIT_FIELD::Set(m_value, value);
    }

    /*
     *  @brief Get the underlying value.
     */
    Type Get()
    {
        return m_value;
    }

    /*
     *  @brief Get the bit field.
     */
    template<typename BIT_FIELD>
    const typename BIT_FIELD::ValueType Get()
    {
        return BIT_FIELD::Get(m_value);
    }

    /*
     *  @brief Get the length of the bitfield.
     * 
     *  @return return length of bitfield.
     */
    static constexpr uint64_t GetLength()
    {
        return m_length;
    }

protected:
    typedef Bitmap<N> BitmapType;                   //< Forward bitmap type to bitfields to access total length.
    typedef Bitmap<N> _Base;                         //< To check if the type is a bitmap.

    static constexpr uint64_t m_offset = 0;         //< Offset of the bit field.
    static constexpr uint64_t m_length = N;         //< Length of the bit field.

    static_assert(m_length <= (sizeof(uint64_t) * CHAR_BIT));  //< Assert if the bitmap size exceeds 64 bits.

    Type    m_value;    // Raw value.

    template <typename, uint64_t>
    friend class BitField;
};

// ---------------------------------------------------------------------------------------------------------

template <typename T>
constexpr std::false_type IsBitmapHelper (T const &);

template <uint64_t N>
constexpr std::true_type IsBitmapHelper (Bitmap<N> const &);

template <typename T>
constexpr auto IsBitmap { decltype(IsBitmapHelper(std::declval<T>()))::value };

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief The bit field class.
 */
template <typename BIT_MAP, uint64_t LENGTH>
class BitField
{
private:
    typedef typename BIT_MAP::BitmapType BitmapType;    //< Foward BitmapType to next BitField.
    typedef BitField<BIT_MAP, LENGTH> _Base;             //< To check if the type is a bitmap.

    static constexpr uint64_t m_length  = LENGTH;                                                                               //< Length of the bit field.
    static constexpr uint64_t m_offset  = (IsBitmap<typename BIT_MAP::_Base>) ? 0 : (BIT_MAP::m_offset + BIT_MAP::m_length);     //< Offset of the bit field.
    static constexpr uint64_t m_maximum = BitTraits<LENGTH>::maxValue;                                                          //< The bit field maximum value.
    static constexpr uint64_t m_mask    = (m_maximum << m_offset);                                                              //< The bit field mask.

    static_assert(BitmapType::m_length >= (m_offset + m_length));           //< Assert if size of bitfields exceeds size of bitmap.

    template <typename, uint64_t>
    friend class BitField;

    template <uint64_t>
    friend class Bitmap;
public:
    typedef typename BitsToIntegralType<LENGTH, false>::Type ValueType;     //< Typedef for the value type.
    typedef typename BitmapType::Type Type;                                 //< Forward the underlying type.

    /*
     *  @brief Set bitfield in the underlying value.
     * 
     *  @param underlyingValue the underlying value.
     *  @param value new value to set.
     */
    static void Set(Type &underlyingValue, const ValueType value)
    {
        underlyingValue &= ~m_mask;
        underlyingValue |= (static_cast<Type>(value) << m_offset);
    }

    /*
     *  @brief Get bitfield from the underlying value.
     * 
     *  @param underlyingValue the underlying value.
     */
    static const ValueType Get(const Type &underlyingValue)
    {
        return ((underlyingValue >> m_offset) & m_maximum);
    }

    /*
     *  @brief Get the length of the bitfield.
     * 
     *  @return return length of bitfield.
     */
    static constexpr uint64_t GetLength()
    {
        return m_length;
    }
};

} // namespace BartOS

#endif // BITFIELDS_H
