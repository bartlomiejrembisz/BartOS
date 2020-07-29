#ifndef REF_COUNTER_H
#define REF_COUNTER_H

#include <stdint.h>

namespace BartOS
{

template <typename TYPE>
class RefPtr;

/*
 *  @brief Class used for creating ref counted objects.
 * 
 *  Usage:
 *  class RefCounted : public RefCounter<RefCounted>
 *  {
 *  public:
 *      void OnDie()
 *      {
 *          std::cout << "DEAD\n";
 *      }
 *  private:
 *      friend class RefCounter<RefCounted>;
 *  };
 */
template <typename TYPE>
class RefCounter
{
public:
    /*
     *  @brief Get the ref count.
     * 
     *  @return the ref count.
     */
    uint16_t GetRefCount() const
    {
        return m_ref;
    }

protected:
    //! Constructor
    RefCounter() : m_ref(0) {}

    //! Copy constructor deleted
    RefCounter(const RefCounter<TYPE> &rhs) = delete;
    RefCounter<TYPE> &operator=(const RefCounter<TYPE> &rhs) = delete;

    //! Move constructor
    RefCounter(RefCounter<TYPE> &&rhs) : m_ref(std::move(rhs.m_ref)) {}

    //! Move assignment operator
    RefCounter<TYPE> &operator=(RefCounter<TYPE> &&rhs)
    {
        m_ref = std::move(rhs.m_ref);

        return *this;
    }

    virtual ~RefCounter() {}

    static void OnDie(RefCounter<TYPE> &object)
    {
        // noop.
    }

protected:
    /*
     *  @brief Increment the refcount.
     */
    void IncrementRefCount()
    {
        ++m_ref;
    }

    /*
     *  @brief Decrement the refcount.
     * 
     *  @return the new refcount.
     */
    uint16_t DecrementRefCount()
    {
        ASSERT(m_ref > 0);
        if (0 == --m_ref)
            TYPE::OnDie(*this);

        return m_ref;
    }   

private:
    uint16_t    m_ref;      ///< The ref count.

    friend class RefPtr<TYPE>;
};

} // namespace BartOS

#endif // REF_COUNTER_H
