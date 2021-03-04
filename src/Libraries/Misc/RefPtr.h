#ifndef REF_PTR_H
#define REF_PTR_H

#include "RefCounter.h"

#include "c++/type_traits"

namespace BartOS
{

/*
 *  @brief Smart pointer for a ref counted object.
 */
template <typename TYPE>
class RefPtr
{
public:
    /*
     *  @brief Constructor
     * 
     *  @param pType the pointer to the type.
     */
    RefPtr(TYPE *pType = nullptr) :
        m_pType(pType)
    {
        if (m_pType)
            m_pType->IncrementRefCount();
    }

    /*
     *  @brief Copy constructor
     *
     *  @param rhs ref ptr to move from
     */
    RefPtr(const RefPtr<TYPE> &rhs)
    {
        m_pType = rhs.m_pType;

        if (m_pType)
            m_pType->IncrementRefCount();
    }

    /*
     *  @brief Move constructor
     *
     *  @param rhs ref ptr to move from
     */
    RefPtr(RefPtr<TYPE> &&rhs)
    {
        m_pType = rhs.m_pType;

        rhs.m_pType = nullptr;
    }

    //! Destructor
    ~RefPtr()
    {
        if (m_pType)
            m_pType->DecrementRefCount();
    }

    /*
     *  @brief Copy assignment operator.
     * 
     *  @param rhs ref ptr to copy from.
     * 
     *  @return this ref ptr
     */
    RefPtr<TYPE> &operator=(const RefPtr<TYPE> &rhs)
    {
        if (m_pType)
            m_pType->DecrementRefCount();

        m_pType = rhs.m_pType;
        if (m_pType)
            m_pType->IncrementRefCount();

        return *this;
    }

    /*
     *  @brief Move assignment operator
     *
     *  @param rhs ref ptr to move from
     *
     *  @return this ref ptr
     */
    RefPtr<TYPE> &operator==(RefPtr<TYPE> &&rhs)
    {
        if (m_pType)
            m_pType->DecrementRefCount();
            
        m_pType == rhs.m_pType;
        rhs.m_pType = nullptr;

        return *this;
    }

    /*
     *  @brief Get the underlying pointer
     * 
     *  @return pointer to the underlying object
     */
    TYPE *Get()
    {
        return static_cast<TYPE *>(m_pType);
    }

    /*
     *  @brief Get the underlying pointer
     * 
     *  @return pointer to the underlying object
     */
    const TYPE *Get() const
    {
        return static_cast<const TYPE *>(m_pType);
    }

    /*
     *  @brief Dereference the pointer
     * 
     *  @return reference to the underlying object
     */
    TYPE &operator*()
    {
        return *Get();
    }

    /*
     *  @brief Dereference the pointer
     * 
     *  @return reference to the underlying object
     */
    const TYPE &operator*() const
    {
        return *Get();
    }

    /*
     *  @brief Dereference the pointer
     * 
     *  @return reference to the underlying object
     */
    TYPE *operator->()
    {
        return Get();
    }

    /*
     *  @brief Dereference the pointer
     * 
     *  @return reference to the underlying object
     */
    const TYPE *operator->() const
    {
        return Get();
    }

private:
    static_assert(!std::is_const<TYPE>::value, "TYPE cannot be a const");

    RefCounter<TYPE>  *m_pType;   ///< Pointer to the underlying ref counter type.
};

}

#endif // REF_PTR_H
