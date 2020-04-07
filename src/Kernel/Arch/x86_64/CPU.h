#ifndef CPU_H
#define CPU_H

#include "Kernel/BartOS.h"
#include "Kernel/multiboot.h"

#include "Libraries/Misc/Singleton.h"

namespace BartOS
{

namespace x86_64
{

namespace CPU
{

/*
 *  @brief Disable interrupts for the lifetime of this object.
 */
class InterruptDisabler
{
public:
    //! Constructor
    InterruptDisabler();

    //! Destructor
    ~InterruptDisabler();
};

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! Disable interrupts.
void Cli();

//! Enable interrupts.
void Sti();

//! Halt the CPU.
void Hlt();

} // namespace CPU

} // namespace x86

} // namespace BartOS

#endif // CPU_H