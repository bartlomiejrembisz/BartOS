#ifndef KERNEL_H
#define KERNEL_H

namespace ROS
{

/*
 *  @brief The Kernel class
 */
class Kernel
{
public:
    /*
     *  @brief The Constructor.
     */
    Kernel();

    /*
     *  @brief The Destructor.
     */
    ~Kernel();

    /*
     *  @brief The kernel main loop function.
     */
    void Run();

private:
    bool        m_isShuttingDown;       ///< Is the kernel shutting down.

};

} // namespace ROS

#endif // KERNEL_H