#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "frg/vector.hpp"
#include "frg/hash.hpp"
#include "frg/hash_map.hpp"
#include "frg/optional.hpp"

#include "Kernel/Memory/Memory.h"

namespace BartOS
{

//! Vector data structure.
template<typename T, typename Allocator = BartOS::Allocator>
using Vector = frg::vector<T, Allocator>;

template<typename T>
using Hash = frg::hash<T>;

template<typename Key, typename Value, typename Hash, typename Allocator = BartOS::Allocator>
using HashMap = frg::hash_map<Key, Value, Hash, Allocator>;

} // namespace BartOS

#endif //! DATA_STRUCTURES_H