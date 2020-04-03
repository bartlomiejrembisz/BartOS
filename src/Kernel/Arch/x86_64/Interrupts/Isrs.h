#ifndef ISRS_H
#define ISRS_H

#include "Kernel/BartOS.h"

namespace BartOS
{

namespace x86_64
{

namespace Isrs
{

//  CODE    ENUMERATION         MNEMONIC                            TYPE            ERROR CODE?
#define ISR_LIST(d) \
    d(0,    DIVIDE_BY_ZERO,     "#DE - Divide By Zero",             "Fault",        false)  \
    d(1,    DEBUG,              "#DB - Debug",                      "Fault/Trap",   false)  \
    d(2,    NMI,                "#   - Non Maskable Interrupt",     "Interrupt",    false)  \
    d(3,    BREAKPOINT,         "#BP - Breakpoint",                 "Trap",         false)  \
    d(4,    OVERFLOW,           "#OF - Overflow",                   "Trap",         false)  \
    d(5,    BOUND_RANGE,        "#BR - Bound Range Exceeded",       "Fault",        false)  \
    d(6,    INVALID_OPCODE,     "#UD - Invalid Opcode",             "Fault",        false)  \
    d(7,    DEV_NOT_AVAILABLE,  "#NM - Device Not Available",       "Fault",        false)  \
    d(8,    DOUBLE_FAULT,       "#DF - Double Fault",               "Abort",        true)   \
    d(9,    DEPRECATED,         "DEPRECATED",                       "",             false)  \
    d(10,   INVALID_TSS,        "#TS - Invalid TSS",                "Fault",        true)   \
    d(11,   SEG_NOT_PRESENT,    "#NP - Segment Not Present",        "Fault",        true)   \
    d(12,   STACK_SEG_FAULT,    "#SS - Stack Segment Fault",        "Fault",        true)   \
    d(13,   GEN_PROT_FAULT,     "#GP - General Protection Fault",   "Fault",        true)   \
    d(14,   PAGE_FAULT,         "#PF - Page Fault",                 "Fault",        true)   \
    d(15,   RESERVED,           "",                                 "",             false)  \
    d(16,   X87_FLOAT,          "#MF - x87 Float Point Exception",  "Fault",        false)  \
    d(17,   ALIGNMENT_CHECK,    "#AC - Alignment Check",            "Fault",        true)   \
    d(18,   MACHINE_CHECK,      "#MC - Machine Check",              "Abort",        false)  \
    d(19,   SIMD_FLOAT,         "#XM - SIMD Float Exception",       "Fault",        false)  \
    d(20,   VIRT,               "#VE - Virtualization Exception",   "Fault",        false)  \
    d(21,   RESERVED1,          "",                                 "",             false)  \
    d(22,   RESERVED2,          "",                                 "",             false)  \
    d(23,   RESERVED3,          "",                                 "",             false)  \
    d(24,   RESERVED4,          "",                                 "",             false)  \
    d(25,   RESERVED5,          "",                                 "",             false)  \
    d(26,   RESERVED6,          "",                                 "",             false)  \
    d(27,   RESERVED7,          "",                                 "",             false)  \
    d(28,   RESERVED8,          "",                                 "",             false)  \
    d(29,   RESERVED9,          "",                                 "",             false)  \
    d(30,   SECURITY_EXCPETION, "#SX - Security Exception",         "",             true)   \
    d(31,   RESERVED10,         "",                                 "",             false)  \

//! Declate the exception enum
#define DECLARE_EXCEPTION_ENUM(code, enumeration, mnemonic, type, errorcode)  const IsrCode JOIN(EXCEPTION_, enumeration) = code;

//! Declare the exception mnemonics
#define DECLARE_EXCEPTION_MNEMONICS(code, enumeration, mnemonic, type, errorcode) mnemonic,

//! Declare the exception type
#define DECLARE_EXCEPTION_TYPE(code, enumeration, mnemonic, type, errorcode) { type },

//! Declare whether the exception contains an error code.
#define DECLARE_EXCEPTION_IS_ERROR_CODE(code, enumeration, mnemonic, type, errorcode) errorcode,

constexpr uint16_t NUM_OF_GATES = 256;

using IsrCode = uint8_t;

ISR_LIST(DECLARE_EXCEPTION_ENUM)

/*
 *  @brief Get the Isr mnemonic
 *
 *  @param isrCode the isr code.
 *
 *  @return the mnemonic.
 */
inline const char *GetIsrMnemonic(const IsrCode isrCode)
{
    static const char *isrToStringArray[] = { ISR_LIST(DECLARE_EXCEPTION_MNEMONICS) };

    return isrToStringArray[isrCode];
}

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get the Isr type
 *
 *  @param isrCode the isr code.
 *
 *  @return the type.
 */
inline const char *GetIsrType(const IsrCode isrCode)
{
    static const char *isrToTypeArray[] = { ISR_LIST(DECLARE_EXCEPTION_TYPE) };

    return isrToTypeArray[isrCode];
}

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get isr is error code present
 *
 *  @param isrCode the isr code.
 *
 *  @return isr error code present.
 */
inline IsrCode GetErrorCodePresent(const IsrCode isrCode)
{
    static const bool isrToErrorCodePresentArray[] = { ISR_LIST(DECLARE_EXCEPTION_IS_ERROR_CODE) };

    return isrToErrorCodePresentArray[isrCode];
}

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Get exception ISR count
 *
 *  @return exception ISR count.
 */
inline IsrCode GetExceptionCount()
{
    constexpr bool isrCountArray[] = { ISR_LIST(DECLARE_EXCEPTION_IS_ERROR_CODE) };

    return ARRAY_SIZE(isrCountArray);
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

// Interrupt Request Handlers:
extern "C" void interrupt_request_handler_0();
extern "C" void interrupt_request_handler_1();
extern "C" void interrupt_request_handler_2();
extern "C" void interrupt_request_handler_3();
extern "C" void interrupt_request_handler_4();
extern "C" void interrupt_request_handler_5();
extern "C" void interrupt_request_handler_6();
extern "C" void interrupt_request_handler_7();
extern "C" void interrupt_request_handler_8();
extern "C" void interrupt_request_handler_9();
extern "C" void interrupt_request_handler_10();
extern "C" void interrupt_request_handler_11();
extern "C" void interrupt_request_handler_12();
extern "C" void interrupt_request_handler_13();
extern "C" void interrupt_request_handler_14();
extern "C" void interrupt_request_handler_15();
extern "C" void interrupt_request_handler_16();
extern "C" void interrupt_request_handler_17();
extern "C" void interrupt_request_handler_18();
extern "C" void interrupt_request_handler_19();
extern "C" void interrupt_request_handler_20();
extern "C" void interrupt_request_handler_21();
extern "C" void interrupt_request_handler_22();
extern "C" void interrupt_request_handler_23();
extern "C" void interrupt_request_handler_24();
extern "C" void interrupt_request_handler_25();
extern "C" void interrupt_request_handler_26();
extern "C" void interrupt_request_handler_27();
extern "C" void interrupt_request_handler_28();
extern "C" void interrupt_request_handler_29();
extern "C" void interrupt_request_handler_30();
extern "C" void interrupt_request_handler_31();
extern "C" void interrupt_request_handler_32();
extern "C" void interrupt_request_handler_33();
extern "C" void interrupt_request_handler_34();
extern "C" void interrupt_request_handler_35();
extern "C" void interrupt_request_handler_36();
extern "C" void interrupt_request_handler_37();
extern "C" void interrupt_request_handler_38();
extern "C" void interrupt_request_handler_39();
extern "C" void interrupt_request_handler_40();
extern "C" void interrupt_request_handler_41();
extern "C" void interrupt_request_handler_42();
extern "C" void interrupt_request_handler_43();
extern "C" void interrupt_request_handler_44();
extern "C" void interrupt_request_handler_45();
extern "C" void interrupt_request_handler_46();
extern "C" void interrupt_request_handler_47();
extern "C" void interrupt_request_handler_48();
extern "C" void interrupt_request_handler_49();
extern "C" void interrupt_request_handler_50();
extern "C" void interrupt_request_handler_51();
extern "C" void interrupt_request_handler_52();
extern "C" void interrupt_request_handler_53();
extern "C" void interrupt_request_handler_54();
extern "C" void interrupt_request_handler_55();
extern "C" void interrupt_request_handler_56();
extern "C" void interrupt_request_handler_57();
extern "C" void interrupt_request_handler_58();
extern "C" void interrupt_request_handler_59();
extern "C" void interrupt_request_handler_60();
extern "C" void interrupt_request_handler_61();
extern "C" void interrupt_request_handler_62();
extern "C" void interrupt_request_handler_63();
extern "C" void interrupt_request_handler_64();
extern "C" void interrupt_request_handler_65();
extern "C" void interrupt_request_handler_66();
extern "C" void interrupt_request_handler_67();
extern "C" void interrupt_request_handler_68();
extern "C" void interrupt_request_handler_69();
extern "C" void interrupt_request_handler_70();
extern "C" void interrupt_request_handler_71();
extern "C" void interrupt_request_handler_72();
extern "C" void interrupt_request_handler_73();
extern "C" void interrupt_request_handler_74();
extern "C" void interrupt_request_handler_75();
extern "C" void interrupt_request_handler_76();
extern "C" void interrupt_request_handler_77();
extern "C" void interrupt_request_handler_78();
extern "C" void interrupt_request_handler_79();
extern "C" void interrupt_request_handler_80();
extern "C" void interrupt_request_handler_81();
extern "C" void interrupt_request_handler_82();
extern "C" void interrupt_request_handler_83();
extern "C" void interrupt_request_handler_84();
extern "C" void interrupt_request_handler_85();
extern "C" void interrupt_request_handler_86();
extern "C" void interrupt_request_handler_87();
extern "C" void interrupt_request_handler_88();
extern "C" void interrupt_request_handler_89();
extern "C" void interrupt_request_handler_90();
extern "C" void interrupt_request_handler_91();
extern "C" void interrupt_request_handler_92();
extern "C" void interrupt_request_handler_93();
extern "C" void interrupt_request_handler_94();
extern "C" void interrupt_request_handler_95();
extern "C" void interrupt_request_handler_96();
extern "C" void interrupt_request_handler_97();
extern "C" void interrupt_request_handler_98();
extern "C" void interrupt_request_handler_99();
extern "C" void interrupt_request_handler_100();
extern "C" void interrupt_request_handler_101();
extern "C" void interrupt_request_handler_102();
extern "C" void interrupt_request_handler_103();
extern "C" void interrupt_request_handler_104();
extern "C" void interrupt_request_handler_105();
extern "C" void interrupt_request_handler_106();
extern "C" void interrupt_request_handler_107();
extern "C" void interrupt_request_handler_108();
extern "C" void interrupt_request_handler_109();
extern "C" void interrupt_request_handler_110();
extern "C" void interrupt_request_handler_111();
extern "C" void interrupt_request_handler_112();
extern "C" void interrupt_request_handler_113();
extern "C" void interrupt_request_handler_114();
extern "C" void interrupt_request_handler_115();
extern "C" void interrupt_request_handler_116();
extern "C" void interrupt_request_handler_117();
extern "C" void interrupt_request_handler_118();
extern "C" void interrupt_request_handler_119();
extern "C" void interrupt_request_handler_120();
extern "C" void interrupt_request_handler_121();
extern "C" void interrupt_request_handler_122();
extern "C" void interrupt_request_handler_123();
extern "C" void interrupt_request_handler_124();
extern "C" void interrupt_request_handler_125();
extern "C" void interrupt_request_handler_126();
extern "C" void interrupt_request_handler_127();
extern "C" void interrupt_request_handler_128();
extern "C" void interrupt_request_handler_129();
extern "C" void interrupt_request_handler_130();
extern "C" void interrupt_request_handler_131();
extern "C" void interrupt_request_handler_132();
extern "C" void interrupt_request_handler_133();
extern "C" void interrupt_request_handler_134();
extern "C" void interrupt_request_handler_135();
extern "C" void interrupt_request_handler_136();
extern "C" void interrupt_request_handler_137();
extern "C" void interrupt_request_handler_138();
extern "C" void interrupt_request_handler_139();
extern "C" void interrupt_request_handler_140();
extern "C" void interrupt_request_handler_141();
extern "C" void interrupt_request_handler_142();
extern "C" void interrupt_request_handler_143();
extern "C" void interrupt_request_handler_144();
extern "C" void interrupt_request_handler_145();
extern "C" void interrupt_request_handler_146();
extern "C" void interrupt_request_handler_147();
extern "C" void interrupt_request_handler_148();
extern "C" void interrupt_request_handler_149();
extern "C" void interrupt_request_handler_150();
extern "C" void interrupt_request_handler_151();
extern "C" void interrupt_request_handler_152();
extern "C" void interrupt_request_handler_153();
extern "C" void interrupt_request_handler_154();
extern "C" void interrupt_request_handler_155();
extern "C" void interrupt_request_handler_156();
extern "C" void interrupt_request_handler_157();
extern "C" void interrupt_request_handler_158();
extern "C" void interrupt_request_handler_159();
extern "C" void interrupt_request_handler_160();
extern "C" void interrupt_request_handler_161();
extern "C" void interrupt_request_handler_162();
extern "C" void interrupt_request_handler_163();
extern "C" void interrupt_request_handler_164();
extern "C" void interrupt_request_handler_165();
extern "C" void interrupt_request_handler_166();
extern "C" void interrupt_request_handler_167();
extern "C" void interrupt_request_handler_168();
extern "C" void interrupt_request_handler_169();
extern "C" void interrupt_request_handler_170();
extern "C" void interrupt_request_handler_171();
extern "C" void interrupt_request_handler_172();
extern "C" void interrupt_request_handler_173();
extern "C" void interrupt_request_handler_174();
extern "C" void interrupt_request_handler_175();
extern "C" void interrupt_request_handler_176();
extern "C" void interrupt_request_handler_177();
extern "C" void interrupt_request_handler_178();
extern "C" void interrupt_request_handler_179();
extern "C" void interrupt_request_handler_180();
extern "C" void interrupt_request_handler_181();
extern "C" void interrupt_request_handler_182();
extern "C" void interrupt_request_handler_183();
extern "C" void interrupt_request_handler_184();
extern "C" void interrupt_request_handler_185();
extern "C" void interrupt_request_handler_186();
extern "C" void interrupt_request_handler_187();
extern "C" void interrupt_request_handler_188();
extern "C" void interrupt_request_handler_189();
extern "C" void interrupt_request_handler_190();
extern "C" void interrupt_request_handler_191();
extern "C" void interrupt_request_handler_192();
extern "C" void interrupt_request_handler_193();
extern "C" void interrupt_request_handler_194();
extern "C" void interrupt_request_handler_195();
extern "C" void interrupt_request_handler_196();
extern "C" void interrupt_request_handler_197();
extern "C" void interrupt_request_handler_198();
extern "C" void interrupt_request_handler_199();
extern "C" void interrupt_request_handler_200();
extern "C" void interrupt_request_handler_201();
extern "C" void interrupt_request_handler_202();
extern "C" void interrupt_request_handler_203();
extern "C" void interrupt_request_handler_204();
extern "C" void interrupt_request_handler_205();
extern "C" void interrupt_request_handler_206();
extern "C" void interrupt_request_handler_207();
extern "C" void interrupt_request_handler_208();
extern "C" void interrupt_request_handler_209();
extern "C" void interrupt_request_handler_210();
extern "C" void interrupt_request_handler_211();
extern "C" void interrupt_request_handler_212();
extern "C" void interrupt_request_handler_213();
extern "C" void interrupt_request_handler_214();
extern "C" void interrupt_request_handler_215();
extern "C" void interrupt_request_handler_216();
extern "C" void interrupt_request_handler_217();
extern "C" void interrupt_request_handler_218();
extern "C" void interrupt_request_handler_219();
extern "C" void interrupt_request_handler_220();
extern "C" void interrupt_request_handler_221();
extern "C" void interrupt_request_handler_222();
extern "C" void interrupt_request_handler_223();
extern "C" void interrupt_request_handler_224();
extern "C" void interrupt_request_handler_225();
extern "C" void interrupt_request_handler_226();
extern "C" void interrupt_request_handler_227();
extern "C" void interrupt_request_handler_228();
extern "C" void interrupt_request_handler_229();
extern "C" void interrupt_request_handler_230();
extern "C" void interrupt_request_handler_231();
extern "C" void interrupt_request_handler_232();
extern "C" void interrupt_request_handler_233();
extern "C" void interrupt_request_handler_234();
extern "C" void interrupt_request_handler_235();
extern "C" void interrupt_request_handler_236();
extern "C" void interrupt_request_handler_237();
extern "C" void interrupt_request_handler_238();
extern "C" void interrupt_request_handler_239();
extern "C" void interrupt_request_handler_240();
extern "C" void interrupt_request_handler_241();
extern "C" void interrupt_request_handler_242();
extern "C" void interrupt_request_handler_243();
extern "C" void interrupt_request_handler_244();
extern "C" void interrupt_request_handler_245();
extern "C" void interrupt_request_handler_246();
extern "C" void interrupt_request_handler_247();
extern "C" void interrupt_request_handler_248();
extern "C" void interrupt_request_handler_249();
extern "C" void interrupt_request_handler_250();
extern "C" void interrupt_request_handler_251();
extern "C" void interrupt_request_handler_252();
extern "C" void interrupt_request_handler_253();
extern "C" void interrupt_request_handler_254();
extern "C" void interrupt_request_handler_255();

} // namespace Isrs

} // namespace x86_64

} // namespace BartOS

#endif // ISRS_H