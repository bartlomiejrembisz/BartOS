#ifndef ITEM_RANGE_H
#define ITEM_RANGE_H

#include "CoreDefs.h"

namespace BartOS
{

template <typename TYPE>
class ItemRange
{
public:
    ItemRange(TYPE* pCollection, const size_t size) :
        m_pCollection(pCollection),
        m_size(size)
    {
    }

    TYPE *begin()
    {
        return &m_pCollection[0];
    }

    const TYPE *begin() const
    {
        return &m_pCollection[0];
    }

    TYPE *end ()
    {
        return &m_pCollection[m_size];
    }

    const TYPE *end () const
    {
        return &m_pCollection[m_size];
    }

    size_t size() const
    {
        return (end() - begin());
    }

private:
    TYPE    *m_pCollection;     ///< The collection pointer.
    size_t  m_size;             ///< The size of the collection.     
};

} // namespace BartOS

#endif // RANGE_H
