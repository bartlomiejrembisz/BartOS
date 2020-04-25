#ifndef REF_PTR_H
#define REF_PTR_H

#include "RefCounter.h"

namespace BartOS
{

/*
 *  @brief Smart pointer for a ref counted object.
 * 
 *  Note: Kind of weird semantics, the RefPtr object needs to be const for const object references *shrug*.
 *        No way to make a const pointer.
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
    explicit RefPtr(RefCounter<TYPE> *pType = nullptr)
    {
        m_pType = pType;
        m_pType->IncrementRefCount();
    }

    RefPtr(const RefPtr<TYPE> &rhs) :
        m_pType(rhs.m_pType)
    {
        m_pType->IncrementRefCount();
    }

    /*
     *  @brief Move constructor
     *
     *  @param rhs ref ptr to move from
     */
    explicit RefPtr(RefPtr<TYPE> &&rhs) :
        m_pType(std::move(rhs.m_pType))
    {
        rhs.m_pType = nullptr;
    }

    /*
     *  @brief Copy constructor
     * 
     *  @param rhs ref ptr to copy from.
     * 
     *  @return this ref ptr
     */
    RefPtr<TYPE> &operator=(const RefPtr<TYPE> &rhs) const
    {
        m_pType = rhs.m_pType;
        m_pType->IncrementRefCount;
    }

    /*
     *  @brief Move assignment
     *
     *  @param rhs ref ptr to move from
     *
     *  @return this ref ptr
     */
    RefPtr<TYPE> &operator==(RefPtr<TYPE> &&rhs) const
    {
        m_pType == std::move(rhs.m_pType);
        rhs.m_pType = nullptr;
    }

    //! Desctructor
    ~RefPtr()
    {
        m_pType->DecrementRefCount();
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
    mutable RefCounter<TYPE>    *m_pType;   ///< Pointer to the underlying ref counter type.
};

}

#endif // REF_PTR_H
