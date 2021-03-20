#ifndef ARRAY_H
#define ARRAY_H

#include <type_traits>
#include <iterator>

namespace BartOS
{

/*
 *  @brief Smart pointer for a ref counted object.
 */
template <class TYPE, size_t SIZE>
class Array
{
public:
    //! Type defs.
    typedef TYPE                                  value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type*                           iterator;
    typedef const value_type*                     const_iterator;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;
    //typedef std::reverse_iterator<iterator>       reverse_iterator;
    //typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //! Iterators
    constexpr iterator begin()                          { return iterator(data()); }
    constexpr const_iterator begin() const              { return const_iterator(data()); }
    constexpr iterator end()                            { return iterator(data() + SIZE); }
    constexpr const_iterator end() const                { return const_iterator(data() + SIZE); }

    // Reverse Iterators
    // constexpr reverse_iterator rbegin()              { return ReverseIterator(end()); }
    // constexpr const_reverse_iterator rbegin() const  { return ConstReverseIterator(end()); }
    // constexpr reverse_iterator rend()                { return ReverseIterator(begin()); }
    // constexpr const_reverse_iterator rend() const    { return ConstReverseIterator(begin()); }

    // Const Iterators
    constexpr const_iterator cbegin() const             { return begin(); }
    constexpr const_iterator cend() const               { return end(); }
    // constexpr const_reverse_iterator crbegin() const { return rbegin(); }
    // constexpr const_reverse_iterator crend() const   { return rend(); }

    //! Get the array size.
    constexpr size_type size() const  { return SIZE; }

    //! Whether the array is empty.
    constexpr bool empty() const      { return false; }

    //! Access operators
    constexpr reference operator[](const size_type index)              { return elements[index]; }
    constexpr const_reference operator[](const size_type index) const  { return elements[index]; }

    constexpr reference front()                 { return elements[0]; }
    constexpr const_reference front() const     { return elements[0]; }
    constexpr reference back()                  { return elements[SIZE - 1]; }
    constexpr const_reference back() const      { return elements[SIZE - 1]; }

    constexpr value_type *data()                { return elements; }
    constexpr const value_type *data() const    { return elements; }

private:
    TYPE elements[SIZE];

    static_assert (SIZE == 0, "Array size needs to be greater than 0.");
};

template <class TYPE, size_t SIZE>
constexpr bool operator==(const Array<TYPE, SIZE> &lhs, const Array<TYPE, SIZE> &rhs)
{
    for (size_t i = 0; i < SIZE; ++i)
    {
      if (lhs[i] != rhs[i])
        return false;
    }
    
    return true;
}

template <class TYPE, size_t SIZE>
constexpr bool operator!=(const Array<TYPE, SIZE> &lhs, const Array<TYPE, SIZE> &rhs)
{
    return !(lhs == rhs);
}

} // namespace BartOS

#endif  // ARRAY_H