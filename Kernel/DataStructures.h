#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <vector>
#include <unordered_map>

#include <functional>
#include <optional>

#include "Lib/Core/Array.h"

#include "Memory/Memory.h"

namespace BartOS
{

//! Vector data structure.
template<typename T>
using Vector = std::vector<T>;

template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = std::allocator< std::pair<const Key, T> >>
using HashMap = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

//! Hash functor.
template<typename T>
using Hash = std::hash<T>;

} // namespace BartOS

#endif //! DATA_STRUCTURES_H
