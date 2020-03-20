#ifndef CORE_DEFS_H
#define CORE_DEFS_H

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "Core/stdlib.h"

namespace ROS
{

// Macros
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#define JOIN(a, b) a##b

#define STRINGIZE(a) #a

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

// Templates

//! Conditional template
template<bool B, class T, class F>
struct conditional { typedef T type; };

// ---------------------------------------------------------------------------------------------------------

template<class T, class F>
struct conditional<false, T, F> { typedef F type; };

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

namespace detail
{
    template<unsigned... digits>
    struct to_chars { static constexpr char value[] = {('0' + digits)..., 0}; };

    template<unsigned... digits>
    constexpr char to_chars<digits...>::value[];;

    template<unsigned rem, unsigned... digits>
    struct explode : explode<rem / 10, rem % 10, digits...> {};

    template<unsigned... digits>
    struct explode<0, digits...> : to_chars<digits...> {};
}

template<uint64_t num>
struct num_to_string : detail::explode<num> {};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Length in bits to integral type.
 */
template<size_t N, bool IS_SIGNED>
class LengthToIntegralType
{
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<8, false>
{
public:
    typedef uint8_t Type;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<16, false>
{
public:
    typedef uint16_t Type;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<32, false>
{
public:
    typedef uint32_t Type;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<64, false>
{
public:
    typedef uint64_t Type;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<8, true>
{
public:
    typedef int8_t type;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<16, true>
{
public:
    typedef int16_t Type;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<32, true>
{
public:
    typedef int32_t Type;
};

// ---------------------------------------------------------------------------------------------------------

template<>
class LengthToIntegralType<64, true>
{
public:
    typedef int64_t Type;
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template <size_t N, bool IS_SIGNED>
class BitsToIntegralType
{
public:
    typedef
            typename conditional<N == 0 , void,
            typename conditional<N <= 8 , typename LengthToIntegralType<8, IS_SIGNED>::Type,
            typename conditional<N <= 16, typename LengthToIntegralType<16, IS_SIGNED>::Type,
            typename conditional<N <= 32, typename LengthToIntegralType<32, IS_SIGNED>::Type,
            typename conditional<N <= 64, typename LengthToIntegralType<64, IS_SIGNED>::Type,
            void>::type>::type>::type>::type>::type Type;

};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template<size_t N>
class BitTraits
{
public:
    typedef typename BitsToIntegralType<N, false>::Type IntegralType;

    static constexpr size_t maxValue = (1 << N) - 1;    //< The max value.
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template<size_t BASE, size_t EXPONENT>
class StaticPower
{
public:
    static constexpr size_t value = BASE * StaticPower<BASE, EXPONENT - 1>::value;
};

// ---------------------------------------------------------------------------------------------------------

template<size_t BASE>
class StaticPower<BASE, 0>
{
public:
    static constexpr size_t value = 1;
};

} // namespace ROS

#endif //! CORE_DEFS_H