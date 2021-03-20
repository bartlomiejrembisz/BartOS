#ifndef UTILITY_H
#define UTILITY_H

#include "CoreDefs.h"

#include <type_traits>

namespace BartOS
{

template <typename T>
constexpr T &Min(const T &a, const T &b)
{
    return (b < a) ? b : a;
}

// ---------------------------------------------------------------------------------------------------------

template <typename T>
constexpr const T &Max(const T &a, const T &b)
{
    return (b > a) ? b : a;
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

namespace TuplePrivate
{

template <std::size_t INDEX, typename TYPE>
struct TupleNode
{
    TYPE    value;
};

template <std::size_t INDEX, typename... ITEMS>
struct TupleProxy;

// Base case
template <std::size_t INDEX>
struct TupleProxy<INDEX> {};

// Recursive specialisation
template <std::size_t INDEX, typename HEAD, typename... TAIL>
struct TupleProxy<INDEX, HEAD, TAIL...> : public TupleNode<INDEX, HEAD>, TupleProxy<INDEX + 1, TAIL...>
{
};

} // namespace TuplePrivate

template <std::size_t INDEX, typename HEAD, typename... TAIL>
inline HEAD &get(TuplePrivate::TupleProxy<INDEX, HEAD, TAIL...> &tuple)
{
    return tuple.TuplePrivate::TupleNode<INDEX, HEAD>::value;
}

template <std::size_t INDEX, typename... ITEMS>
using Tuple = TuplePrivate::TupleProxy<0, ITEMS...>;

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

template <typename T1, typename T2>
struct Pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    explicit constexpr Pair() :
        first(), second() 
    {
    }

    constexpr explicit Pair(T1 const& t1, T2 const& t2) :
        first(t1), second(t2)
    {
    }

    template <typename U1, typename U2>
    constexpr explicit Pair(U1 &&u1, U2 &&u2) :
        first(std::forward<U1>(u1)), second(std::forward<U2>(u2))
    {
    }

    template <typename U1, typename U2>
    constexpr explicit Pair(Pair<U1, U2> const &pair) :
        first(pair.first), second(pair.second)
    {
    }

    template<typename Tuple>
    constexpr explicit Pair(Tuple &&pair) :
        first(BartOS::get<0>(std::forward<Tuple>(pair))),
        second(BartOS::get<1>(std::forward<Tuple>(pair)))
    {
    }

    Pair &operator=(typename std::conditional<
                        std::is_copy_assignable<first_type>::value &&
                        std::is_copy_assignable<second_type>::value,
                    Pair, void>::type const &pair)
    {
        first = pair.first;
        second = pair.second;
        return *this;
    }

    Pair &operator=(typename std::conditional<
                        std::is_move_assignable<first_type>::value &&
                        std::is_move_assignable<second_type>::value,
                    Pair, void>::type &&pair)
    {
        first = std::forward<first_type>(pair.first);
        second = std::forward<second_type>(pair.second);
        return *this;
    }

    template<typename Tuple>
    Pair &operator=(Tuple &&pair) {
        first = BartOS::get<0>(std::forward<Tuple>(pair));
        second = BartOS::get<1>(std::forward<Tuple>(pair));
        return *this;
    }

    constexpr bool operator==(const Pair<T1, T2> &rhs)
    {
        return ((first == rhs.first) && (second == rhs.second));
    }

    constexpr bool operator!=(const Pair<T1, T2> &rhs)
    {
        return !(*this == rhs);
    }

    T1 first;
    T2 second;
};

template <class T1, class T2>
inline Pair<T1, T2> make_pair(T1 t1, T2 t2)
{
    return Pair<T1, T2>(t1, t2);
}

} // namespace BartOS

#endif  // UTILITY_H