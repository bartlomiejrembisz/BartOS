#ifndef CPU_H
#define CPU_H

#include "Kernel/BartOS.h"

#include "Libraries/Misc/BitFields.h"

namespace BartOS
{

inline namespace x86_64
{

namespace CPU
{

//! The RFLAGS register bitmap.
class [[gnu::packed]] RFLAGS : public Bitmap<64>
{
public:
    typedef BitField<RFLAGS, 1> CarryFlag;                                  ///< The carry flag.
    typedef BitField<CarryFlag, 1> Reserved;                                ///< Reserved.
    typedef BitField<Reserved, 1> ParityFlag;                               ///< The parity flag.
    typedef BitField<ParityFlag, 1> Reserved2;                              ///< Reserved.
    typedef BitField<Reserved2, 1> AuxilaryCarryFlag;                       ///< The auxilary carry flag.
    typedef BitField<AuxilaryCarryFlag, 1> Reserved3;                       ///< Reserved.
    typedef BitField<Reserved3, 1> ZeroFlag;                                ///< The zero flag.
    typedef BitField<ZeroFlag, 1> SignFlag;                                 ///< The sign flag.
    typedef BitField<SignFlag, 1> TrapFlag;                                 ///< The trap flag.
    typedef BitField<TrapFlag, 1> InterruptEnableFlag;                      ///< The interrupt enable flag.
    typedef BitField<InterruptEnableFlag, 1> DirectionFlag;                 ///< The direction flag.
    typedef BitField<DirectionFlag, 1> OverflowFlag;                        ///< The overflow flag.
    typedef BitField<OverflowFlag, 2> IOPrivilegeLevel;                     ///< The IO privilege level.
    typedef BitField<IOPrivilegeLevel, 1> NestedTask;                       ///< The nested task flag.
    typedef BitField<NestedTask, 1> Reserved4;                              ///< Reserved.
    typedef BitField<Reserved4, 1> ResumeFlag;                              ///< The resume flag.
    typedef BitField<ResumeFlag, 1> Virtual8086Mode;                        ///< The virtual 8086 mode flag.
    typedef BitField<Virtual8086Mode, 1> AlignmentCheck;                    ///< The alignment check / access control flag.
    typedef BitField<AlignmentCheck, 1> VirtIntFlag;                        ///< The virtual interrupt flag.
    typedef BitField<VirtIntFlag, 1> VirtIntPending;                        ///< The virtual interrupt pending flag.
    typedef BitField<VirtIntFlag, 1> IDFlag;                                ///< The ID flag.
    typedef BitField<IDFlag, 42> Reserved5;                                 ///< Reserved.
};
static_assert(sizeof(RFLAGS) == 8);

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get the RFLAGS register content.
 * 
 *  @return the content of the RFLAGS register.
 */
RFLAGS GetRFLAGS();

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Set the RFLAGS register.
 * 
 *  @param rflags the new rflags.
 */
void SetRFLAGS(const RFLAGS rflags);

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! The Control Register 0 bitmap.
class CR0 : public Bitmap<64>
{
public:
    typedef BitField<CR0, 1> ProtectedMode;                 ///< Enable protected mode.
    typedef BitField<ProtectedMode, 1> MonitorCoProcessor;  ///< Controls interraction of WAIR/FWAIT inst with TS flag.
    typedef BitField<MonitorCoProcessor, 1> Emulation;      ///< If clear x87 FPU is present.
    typedef BitField<Emulation, 1> TaskSwitched;            ///< Allows saving x87 task context upon task switch.
    typedef BitField<TaskSwitched, 1> ExtensionType;        ///< On the 386, it allowed to specify whether the external math coprocessor was an 80287 or 80387.
    typedef BitField<ExtensionType, 1> NumericError;        ///< Enable internal x87 floating point error reporting when set.
    typedef BitField<NumericError, 1> WriteProtect;         ///< When set, the CPU can't write to read-only pages when privilege level is 0.
    typedef BitField<WriteProtect, 1> AlignmentMask;        ///< Alignment check enabled if AM set, AC flag (in EFLAGS register) set and ring 3.
    typedef BitField<AlignmentMask, 1> NotWriteThrough;     ///< Globally enables/disables write-through caching.
    typedef BitField<NotWriteThrough, 1> CacheDisable;      ///< Globally enables/disables the memory cache.
    typedef BitField<CacheDisable, 1> Paging;               ///< Enable paging.
};
static_assert(sizeof(CR0) == 8);

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get the CR0 register content.
 * 
 *  @return the content of the CR0 register.
 */
CR0 GetCR0();

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Set the CR0 register.
 * 
 *  @param cr0 the new CR0 content.
 */
void SetCR0(const CR0 cr0);

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! The Control Register 3 bitmap.
class CR3 : public Bitmap<64>
{
public:
    typedef BitField<CR3, 12> Reserved;                 ///< Reserved.
    typedef BitField<Reserved, 52> PhysicalAddress;     ///< The page aligned physical address of the PML4.
};
static_assert(sizeof(CR3) == 8);

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get the CR3 register content.
 * 
 *  @return the content of the CR3 register.
 */
CR3 GetCR3();

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Set the CR3 register.
 * 
 *  @param cr3 the new CR3 content.
 */
void SetCR3(const CR3 cr3);

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! The Control Register 4 bitmap.
class CR4 : public Bitmap<64>
{
public:
    typedef BitField<CR4, 1> Virt8086ModeExt;                           ///< The Virtual-8086 mode extensions.
    typedef BitField<Virt8086ModeExt, 1> PMVirtInt;                     ///< The Protected Mode Virtual Interrupts flag.
    typedef BitField<PMVirtInt, 1> TimestampDisable;                    ///< The RDTSC instruction can only be executed when in rin 0.
    typedef BitField<TimestampDisable, 1> DebugExt;                     ///< If set, enabled debug register based breaks on IO space access.
    typedef BitField<DebugExt, 1> PageSizeExtension;                    ///< Enables 4MiB pages. Ignored in long mode.
    typedef BitField<PageSizeExtension, 1> PAE;                         ///< The physical address extension flag (36 bit physical addressing).
    typedef BitField<PAE, 1> MachineCheckExteption;                     ///< Enables machine check interrupts to occur.
    typedef BitField<MachineCheckExteption, 1> PageGlobalEnabled;       ///< If set, address translations may be shared between address spaces.
    typedef BitField<PageGlobalEnabled, 1> PerfMonCounterEnabled;       ///< If set, RDPMC can be executed at any privilege level, else ring 0.
    typedef BitField<PerfMonCounterEnabled, 1> OSFXSR;                  ///< Operating system support for FXSAVE and FXRSTOR instructions.
    typedef BitField<OSFXSR, 1> OSXMMEXCPT;                             ///< Operating system support for unmasked SIMD floating point exceptions.
    typedef BitField<OSXMMEXCPT, 1> UMIP;                               ///< User mode instruction prevention.
    typedef BitField<UMIP, 1> LA57;                                     ///< Enabled 5 level paging.
    typedef BitField<LA57, 1> VMXE;                                     ///< Virtual machine extensions enable.
    typedef BitField<VMXE, 1> SMXE;                                     ///< Safer mode extensions enable.
    typedef BitField<SMXE, 1> FSGSBASE;                                 ///< Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE and WRGSBASE.
    typedef BitField<FSGSBASE, 1> PCIDE;                                ///< PCID enable.
    typedef BitField<PCIDE, 1> OSXSAVE;                                 ///< XSAVE and Processor Extended States Enable.
    typedef BitField<OSXSAVE, 1> SMEP;                                  ///< Supervisor mode execution prevention enable.
    typedef BitField<SMEP, 1> SMAP;                                     ///< Supervisor mode access prevention enable.
    typedef BitField<SMAP, 1> PKE;                                      ///< Protection key enable.
    typedef BitField<PKE, 42> Reserved;                                 ///< Reserved.
};
static_assert(sizeof(CR4) == 8);

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get the CR4 register content.
 * 
 *  @return the content of the CR4 register.
 */
CR4 GetCR4();

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Set the CR4 register.
 * 
 *  @param cr4 the new CR4 content.
 */
void SetCR4(const CR4 cr4);

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//! The EFER model-specific register.
class EFER : public Bitmap<64>
{
public:
    typedef BitField<EFER, 1> SysCallExt;                   ///< The system call extensions flag.
    typedef BitField<SysCallExt, 8> Reserved;               ///< Reserved.
    typedef BitField<Reserved, 1> LongModeEnable;           ///< The long mode enable flag.
    typedef BitField<LongModeEnable, 1> LongModeActive;     ///< The long mode active flag.
    typedef BitField<LongModeActive, 1> NoExecuteEnable;    ///< The no execute enable flag.
    typedef BitField<NoExecuteEnable, 1> SVME;              ///< The secure virtual machine enable flag.
    typedef BitField<SVME, 1> LMSLE;                        ///< The long mode segment limit enable flag.
    typedef BitField<LMSLE, 1> FFXSR;                       ///< The fast FXSAVE/FXRSTOR flag.
    typedef BitField<FFXSR, 1> TCE;                         ///< The translation cache extension flag.
    typedef BitField<TCE, 48> Reserved2;                    ///< Reserved.
};
static_assert(sizeof(EFER) == 8);

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get the MSR register content.
 * 
 *  @param msr the MSR id
 *  @return the content of the MSR register.
 */
uint64_t GetMSR(const uint32_t msrId);

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Set the MSR register.
 * 
 *  @param msr the MSR id
 *  @param value the new MSR register content.
 */
void SetEFER(const uint32_t msrId, const uint64_t value);

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

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