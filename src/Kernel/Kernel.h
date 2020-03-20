#ifndef KERNEL_H
#define KERNEL_H

#include "Core/ROseless.h"
#include "multiboot.h"

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
        Kernel(const multiboot_info_t * const pMultibootInfo);

    /*
     *  @brief The Destructor.
     */
    ~Kernel();

    /*
     *  @brief The kernel main loop function.
     */
    void Run();

private:
    const multiboot_info_t * const  m_pMultibootInfo;       ///< Multiboot info object.
    bool                            m_isShuttingDown;       ///< Is the kernel shutting down.
};

} // namespace ROS

#endif // KERNEL_H