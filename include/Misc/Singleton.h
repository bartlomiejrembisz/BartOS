#ifndef SINGLETON_H
#define SINGLETON_H

template <typename CLASS>
class Singleton
{
public:
    static CLASS &GetInstance();

protected:
    Singleton();

private:
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
CLASS &Singleton<CLASS>::GetInstance()
{
    return m_singleton;
}

#endif // SINGLETON_H