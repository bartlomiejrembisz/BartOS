#ifndef CPU_H
#define CPU_H

#include "Kernel/BartOS.h"
#include "Kernel/multiboot.h"

#include "Libraries/Misc/Singleton.h"

namespace BartOS
{

namespace x86_64
{

/*
 *  @brief The CPU class
 */
class CPU
{
public:
    //! Abort the execution of the kernel.
    static void Abort();

    //! Disable interrupts.
    static void Cli();

    //! Enable interrupts.
    static void Sti();

    //! Halt the CPU.
    static void Hlt();
};

} // namespace x86

} // namespace BartOS

#endif // CPU_H