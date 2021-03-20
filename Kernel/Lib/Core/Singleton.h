#ifndef SINGLETON_H
#define SINGLETON_H

/*
 *  Usage:
 * 
 * class MySingleton : public Singleton<MySingleton>
 * {
 *  private:
 *      //! Private constructor
 *      MySingleton();
 * 
 *      friend class Singleton<MySingleton>;
 * }
 * 
 * 
 * MySingleton::GetInstance();
 */


/*
 *  @brief The Singleton class.
 */
template <typename CLASS>
class Singleton
{
public:
    static CLASS &Get();

protected:
    Singleton();

private:
    Singleton(const Singleton &rhs) = delete;
    Singleton(Singleton &&rhs) = delete;

    Singleton &operator=(const Singleton &rhs) = delete;
    Singleton &operator=(Singleton &&rhs) = delete;

    static CLASS m_singleton;
};

// ---------------------------------------------------------------------------------------------------------

template <typename CLASS>
CLASS Singleton<CLASS>::m_singleton;

// ---------------------------------------------------------------------------------------------------------

template <typename CLASS>
Singleton<CLASS>::Singleton()
{

}

// ---------------------------------------------------------------------------------------------------------

template <typename CLASS>
CLASS &Singleton<CLASS>::Get()
{
    return m_singleton;
}

#endif // SINGLETON_H