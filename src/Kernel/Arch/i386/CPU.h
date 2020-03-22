#ifndef CPU_H
#define CPU_H

#include "Kernel/BartOS.h"
#include "Kernel/multiboot.h"

#include "Libraries/Misc/Singleton.h"

#include "GDT.h"

namespace BartOS
{

namespace x86
{

/*
 *  @brief The CPU class
 */
class CPU : public Singleton<CPU>
{
public:
    static const multiboot_info_t *m_pMultibootInfo;       ///< Multiboot info object.

    /*
     *  @brief Initialize the global descriptor table for the cpu.
     */
    void InitializeGdt();

    /*
     *  @brief Initialize the interrupt descriptor table for the cpu.
     */
    void InitializeIdt();

    //! Abort the execution of the kernel.
    static void Abort();

    //! Disable interrupts.
    static void Cli();

    //! Enable interrupts.
    static void Sti();

    //! Halt the CPU.
    static void Hlt();
private:
    /*
     *  @brief The Constructor.
     */
    CPU();

    /*
     *  @brief The Destructor.
     */
    ~CPU();

    GlobalDescriptorTable   m_globalDescriptorTable;    //< The global descriptor table for the CPU.

    friend class Singleton<CPU>;
};

} // namespace x86

} // namespace BartOS

#endif // CPU_H