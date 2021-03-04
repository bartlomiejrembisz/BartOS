#include "InterruptDescriptorTable.h"

#include "Kernel/Arch/x86_64/GDT.h"
#include "Kernel/Arch/x86_64/x86_64.h"
#include "Kernel/Arch/x86_64/CPU.h"

#include "Isrs.h"

namespace BartOS
{

namespace x86_64
{

namespace
{

//! IDT Entry options.
class Options : public Bitmap<16>
{
public:
    typedef BitField<Options, 3> InterruptStackTable;
    typedef BitField<InterruptStackTable, 5> Reserved;
    typedef BitField<Reserved, 4> Type;
    typedef BitField<Type, 1> MustBeZero;
    typedef BitField<MustBeZero, 2> PrivilegeLevel;
    typedef BitField<PrivilegeLevel, 1> Present;
};

// ---------------------------------------------------------------------------------------------------------

/*
 *  @brief Create the options field in Gate::Word1.
 *
 *  @param interruptStackTable the interrupt stack table.
 *  @param type the gate type.
 *  @param level the privilege level (ring).
 *  @param isPresent is the entry present.
 *
 *  @return options.
 */
constexpr Gate::Word1::Options::ValueType CreateOptions(const uint8_t interruptStackTable, const uint8_t type, const uint8_t level,
        const bool isPresent)
{
    Options options;

    options.Set<Options::InterruptStackTable>(interruptStackTable);
    options.Set<Options::Type>(type);
    options.Set<Options::MustBeZero>(0);
    options.Set<Options::PrivilegeLevel>(level);
    options.Set<Options::Present>(isPresent & 0b1);

    return options.Get();
}

// ---------------------------------------------------------------------------------------------------------

// Interrupt gate types.
constexpr uint8_t TASK_GATE_TYPE_32         = 0x5;
constexpr uint8_t INTERRUPT_GATE_TYPE_16    = 0x6;
constexpr uint8_t TRAP_GATE_TYPE_16         = 0x7;
constexpr uint8_t INTERRUPT_GATE_TYPE_32    = 0xE;
constexpr uint8_t TRAP_GATE_TYPE_32         = 0xF;

} // namespace

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

Gate::Gate()
{
}

// ---------------------------------------------------------------------------------------------------------

Gate::~Gate()
{
}

// ---------------------------------------------------------------------------------------------------------

void Gate::Set(void(*isr_handler)(), const uint8_t interruptStackTable)
{
    Word1 word1;
    Word2 word2;

    //! Setup function pointer.
    word1.Set<Word1::LowerFunPtr>(((Address_t) isr_handler) & 0xFFFF);
    word1.Set<Word1::MiddleFunPtr>(((Address_t) isr_handler) >>  Word1::LowerFunPtr::GetLength());
    const uint32_t remainingFunPtr = ((Address_t) isr_handler) >> (Word1::LowerFunPtr::GetLength() + Word1::MiddleFunPtr::GetLength());
    word2.Set<Word2::RemainingFunPtr>(remainingFunPtr & 0xFFFFFFFF);

    //! Setup GDT selctor and options.
    word1.Set<Word1::GDTSelector>(GDT::GetSelectorOffset(GDT::KERNEL_CODE));
    word1.Set<Word1::Options>(CreateOptions(interruptStackTable, INTERRUPT_GATE_TYPE_32, 0, true));

    m_word1 = word1.Get();
    m_word2 = word2.Get();
}

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

InterruptDescriptorTable::InterruptDescriptorTable() :
    m_descriptor { .m_size = sizeof(m_gates), .m_address = PhysicalAddress((Address_t) &m_gates) }
{
}

// ---------------------------------------------------------------------------------------------------------

InterruptDescriptorTable::~InterruptDescriptorTable()
{
}

// ---------------------------------------------------------------------------------------------------------

void InterruptDescriptorTable::Initialize()
{
    m_gates[0].Set(interrupt_request_handler_0, 0);
    m_gates[1].Set(interrupt_request_handler_1, 0);
    m_gates[2].Set(interrupt_request_handler_2, 0);
    m_gates[3].Set(interrupt_request_handler_3, 0);
    m_gates[4].Set(interrupt_request_handler_4, 0);
    m_gates[5].Set(interrupt_request_handler_5, 0);
    m_gates[6].Set(interrupt_request_handler_6, 0);
    m_gates[7].Set(interrupt_request_handler_7, 0);
    m_gates[8].Set(interrupt_request_handler_8, 0);
    m_gates[9].Set(interrupt_request_handler_9, 0);
    m_gates[10].Set(interrupt_request_handler_10, 0);
    m_gates[11].Set(interrupt_request_handler_11, 0);
    m_gates[12].Set(interrupt_request_handler_12, 0);
    m_gates[13].Set(interrupt_request_handler_13, 0);
    m_gates[14].Set(interrupt_request_handler_14, 0);
    m_gates[15].Set(interrupt_request_handler_15, 0);
    m_gates[16].Set(interrupt_request_handler_16, 0);
    m_gates[17].Set(interrupt_request_handler_17, 0);
    m_gates[18].Set(interrupt_request_handler_18, 0);
    m_gates[19].Set(interrupt_request_handler_19, 0);
    m_gates[20].Set(interrupt_request_handler_20, 0);
    m_gates[21].Set(interrupt_request_handler_21, 0);
    m_gates[22].Set(interrupt_request_handler_22, 0);
    m_gates[23].Set(interrupt_request_handler_23, 0);
    m_gates[24].Set(interrupt_request_handler_24, 0);
    m_gates[25].Set(interrupt_request_handler_25, 0);
    m_gates[26].Set(interrupt_request_handler_26, 0);
    m_gates[27].Set(interrupt_request_handler_27, 0);
    m_gates[28].Set(interrupt_request_handler_28, 0);
    m_gates[29].Set(interrupt_request_handler_29, 0);
    m_gates[30].Set(interrupt_request_handler_30, 0);
    m_gates[31].Set(interrupt_request_handler_31, 0);
    m_gates[32].Set(interrupt_request_handler_32, 0);
    m_gates[33].Set(interrupt_request_handler_33, 0);
    m_gates[34].Set(interrupt_request_handler_34, 0);
    m_gates[35].Set(interrupt_request_handler_35, 0);
    m_gates[36].Set(interrupt_request_handler_36, 0);
    m_gates[37].Set(interrupt_request_handler_37, 0);
    m_gates[38].Set(interrupt_request_handler_38, 0);
    m_gates[39].Set(interrupt_request_handler_39, 0);
    m_gates[40].Set(interrupt_request_handler_40, 0);
    m_gates[41].Set(interrupt_request_handler_41, 0);
    m_gates[42].Set(interrupt_request_handler_42, 0);
    m_gates[43].Set(interrupt_request_handler_43, 0);
    m_gates[44].Set(interrupt_request_handler_44, 0);
    m_gates[45].Set(interrupt_request_handler_45, 0);
    m_gates[46].Set(interrupt_request_handler_46, 0);
    m_gates[47].Set(interrupt_request_handler_47, 0);
    m_gates[48].Set(interrupt_request_handler_48, 0);
    m_gates[49].Set(interrupt_request_handler_49, 0);
    m_gates[50].Set(interrupt_request_handler_50, 0);
    m_gates[51].Set(interrupt_request_handler_51, 0);
    m_gates[52].Set(interrupt_request_handler_52, 0);
    m_gates[53].Set(interrupt_request_handler_53, 0);
    m_gates[54].Set(interrupt_request_handler_54, 0);
    m_gates[55].Set(interrupt_request_handler_55, 0);
    m_gates[56].Set(interrupt_request_handler_56, 0);
    m_gates[57].Set(interrupt_request_handler_57, 0);
    m_gates[58].Set(interrupt_request_handler_58, 0);
    m_gates[59].Set(interrupt_request_handler_59, 0);
    m_gates[60].Set(interrupt_request_handler_60, 0);
    m_gates[61].Set(interrupt_request_handler_61, 0);
    m_gates[62].Set(interrupt_request_handler_62, 0);
    m_gates[63].Set(interrupt_request_handler_63, 0);
    m_gates[64].Set(interrupt_request_handler_64, 0);
    m_gates[65].Set(interrupt_request_handler_65, 0);
    m_gates[66].Set(interrupt_request_handler_66, 0);
    m_gates[67].Set(interrupt_request_handler_67, 0);
    m_gates[68].Set(interrupt_request_handler_68, 0);
    m_gates[69].Set(interrupt_request_handler_69, 0);
    m_gates[70].Set(interrupt_request_handler_70, 0);
    m_gates[71].Set(interrupt_request_handler_71, 0);
    m_gates[72].Set(interrupt_request_handler_72, 0);
    m_gates[73].Set(interrupt_request_handler_73, 0);
    m_gates[74].Set(interrupt_request_handler_74, 0);
    m_gates[75].Set(interrupt_request_handler_75, 0);
    m_gates[76].Set(interrupt_request_handler_76, 0);
    m_gates[77].Set(interrupt_request_handler_77, 0);
    m_gates[78].Set(interrupt_request_handler_78, 0);
    m_gates[79].Set(interrupt_request_handler_79, 0);
    m_gates[80].Set(interrupt_request_handler_80, 0);
    m_gates[81].Set(interrupt_request_handler_81, 0);
    m_gates[82].Set(interrupt_request_handler_82, 0);
    m_gates[83].Set(interrupt_request_handler_83, 0);
    m_gates[84].Set(interrupt_request_handler_84, 0);
    m_gates[85].Set(interrupt_request_handler_85, 0);
    m_gates[86].Set(interrupt_request_handler_86, 0);
    m_gates[87].Set(interrupt_request_handler_87, 0);
    m_gates[88].Set(interrupt_request_handler_88, 0);
    m_gates[89].Set(interrupt_request_handler_89, 0);
    m_gates[90].Set(interrupt_request_handler_90, 0);
    m_gates[91].Set(interrupt_request_handler_91, 0);
    m_gates[92].Set(interrupt_request_handler_92, 0);
    m_gates[93].Set(interrupt_request_handler_93, 0);
    m_gates[94].Set(interrupt_request_handler_94, 0);
    m_gates[95].Set(interrupt_request_handler_95, 0);
    m_gates[96].Set(interrupt_request_handler_96, 0);
    m_gates[97].Set(interrupt_request_handler_97, 0);
    m_gates[98].Set(interrupt_request_handler_98, 0);
    m_gates[99].Set(interrupt_request_handler_99, 0);
    m_gates[100].Set(interrupt_request_handler_100, 0);
    m_gates[101].Set(interrupt_request_handler_101, 0);
    m_gates[102].Set(interrupt_request_handler_102, 0);
    m_gates[103].Set(interrupt_request_handler_103, 0);
    m_gates[104].Set(interrupt_request_handler_104, 0);
    m_gates[105].Set(interrupt_request_handler_105, 0);
    m_gates[106].Set(interrupt_request_handler_106, 0);
    m_gates[107].Set(interrupt_request_handler_107, 0);
    m_gates[108].Set(interrupt_request_handler_108, 0);
    m_gates[109].Set(interrupt_request_handler_109, 0);
    m_gates[110].Set(interrupt_request_handler_110, 0);
    m_gates[111].Set(interrupt_request_handler_111, 0);
    m_gates[112].Set(interrupt_request_handler_112, 0);
    m_gates[113].Set(interrupt_request_handler_113, 0);
    m_gates[114].Set(interrupt_request_handler_114, 0);
    m_gates[115].Set(interrupt_request_handler_115, 0);
    m_gates[116].Set(interrupt_request_handler_116, 0);
    m_gates[117].Set(interrupt_request_handler_117, 0);
    m_gates[118].Set(interrupt_request_handler_118, 0);
    m_gates[119].Set(interrupt_request_handler_119, 0);
    m_gates[120].Set(interrupt_request_handler_120, 0);
    m_gates[121].Set(interrupt_request_handler_121, 0);
    m_gates[122].Set(interrupt_request_handler_122, 0);
    m_gates[123].Set(interrupt_request_handler_123, 0);
    m_gates[124].Set(interrupt_request_handler_124, 0);
    m_gates[125].Set(interrupt_request_handler_125, 0);
    m_gates[126].Set(interrupt_request_handler_126, 0);
    m_gates[127].Set(interrupt_request_handler_127, 0);
    m_gates[128].Set(interrupt_request_handler_128, 0);
    m_gates[129].Set(interrupt_request_handler_129, 0);
    m_gates[130].Set(interrupt_request_handler_130, 0);
    m_gates[131].Set(interrupt_request_handler_131, 0);
    m_gates[132].Set(interrupt_request_handler_132, 0);
    m_gates[133].Set(interrupt_request_handler_133, 0);
    m_gates[134].Set(interrupt_request_handler_134, 0);
    m_gates[135].Set(interrupt_request_handler_135, 0);
    m_gates[136].Set(interrupt_request_handler_136, 0);
    m_gates[137].Set(interrupt_request_handler_137, 0);
    m_gates[138].Set(interrupt_request_handler_138, 0);
    m_gates[139].Set(interrupt_request_handler_139, 0);
    m_gates[140].Set(interrupt_request_handler_140, 0);
    m_gates[141].Set(interrupt_request_handler_141, 0);
    m_gates[142].Set(interrupt_request_handler_142, 0);
    m_gates[143].Set(interrupt_request_handler_143, 0);
    m_gates[144].Set(interrupt_request_handler_144, 0);
    m_gates[145].Set(interrupt_request_handler_145, 0);
    m_gates[146].Set(interrupt_request_handler_146, 0);
    m_gates[147].Set(interrupt_request_handler_147, 0);
    m_gates[148].Set(interrupt_request_handler_148, 0);
    m_gates[149].Set(interrupt_request_handler_149, 0);
    m_gates[150].Set(interrupt_request_handler_150, 0);
    m_gates[151].Set(interrupt_request_handler_151, 0);
    m_gates[152].Set(interrupt_request_handler_152, 0);
    m_gates[153].Set(interrupt_request_handler_153, 0);
    m_gates[154].Set(interrupt_request_handler_154, 0);
    m_gates[155].Set(interrupt_request_handler_155, 0);
    m_gates[156].Set(interrupt_request_handler_156, 0);
    m_gates[157].Set(interrupt_request_handler_157, 0);
    m_gates[158].Set(interrupt_request_handler_158, 0);
    m_gates[159].Set(interrupt_request_handler_159, 0);
    m_gates[160].Set(interrupt_request_handler_160, 0);
    m_gates[161].Set(interrupt_request_handler_161, 0);
    m_gates[162].Set(interrupt_request_handler_162, 0);
    m_gates[163].Set(interrupt_request_handler_163, 0);
    m_gates[164].Set(interrupt_request_handler_164, 0);
    m_gates[165].Set(interrupt_request_handler_165, 0);
    m_gates[166].Set(interrupt_request_handler_166, 0);
    m_gates[167].Set(interrupt_request_handler_167, 0);
    m_gates[168].Set(interrupt_request_handler_168, 0);
    m_gates[169].Set(interrupt_request_handler_169, 0);
    m_gates[170].Set(interrupt_request_handler_170, 0);
    m_gates[171].Set(interrupt_request_handler_171, 0);
    m_gates[172].Set(interrupt_request_handler_172, 0);
    m_gates[173].Set(interrupt_request_handler_173, 0);
    m_gates[174].Set(interrupt_request_handler_174, 0);
    m_gates[175].Set(interrupt_request_handler_175, 0);
    m_gates[176].Set(interrupt_request_handler_176, 0);
    m_gates[177].Set(interrupt_request_handler_177, 0);
    m_gates[178].Set(interrupt_request_handler_178, 0);
    m_gates[179].Set(interrupt_request_handler_179, 0);
    m_gates[180].Set(interrupt_request_handler_180, 0);
    m_gates[181].Set(interrupt_request_handler_181, 0);
    m_gates[182].Set(interrupt_request_handler_182, 0);
    m_gates[183].Set(interrupt_request_handler_183, 0);
    m_gates[184].Set(interrupt_request_handler_184, 0);
    m_gates[185].Set(interrupt_request_handler_185, 0);
    m_gates[186].Set(interrupt_request_handler_186, 0);
    m_gates[187].Set(interrupt_request_handler_187, 0);
    m_gates[188].Set(interrupt_request_handler_188, 0);
    m_gates[189].Set(interrupt_request_handler_189, 0);
    m_gates[190].Set(interrupt_request_handler_190, 0);
    m_gates[191].Set(interrupt_request_handler_191, 0);
    m_gates[192].Set(interrupt_request_handler_192, 0);
    m_gates[193].Set(interrupt_request_handler_193, 0);
    m_gates[194].Set(interrupt_request_handler_194, 0);
    m_gates[195].Set(interrupt_request_handler_195, 0);
    m_gates[196].Set(interrupt_request_handler_196, 0);
    m_gates[197].Set(interrupt_request_handler_197, 0);
    m_gates[198].Set(interrupt_request_handler_198, 0);
    m_gates[199].Set(interrupt_request_handler_199, 0);
    m_gates[200].Set(interrupt_request_handler_200, 0);
    m_gates[201].Set(interrupt_request_handler_201, 0);
    m_gates[202].Set(interrupt_request_handler_202, 0);
    m_gates[203].Set(interrupt_request_handler_203, 0);
    m_gates[204].Set(interrupt_request_handler_204, 0);
    m_gates[205].Set(interrupt_request_handler_205, 0);
    m_gates[206].Set(interrupt_request_handler_206, 0);
    m_gates[207].Set(interrupt_request_handler_207, 0);
    m_gates[208].Set(interrupt_request_handler_208, 0);
    m_gates[209].Set(interrupt_request_handler_209, 0);
    m_gates[210].Set(interrupt_request_handler_210, 0);
    m_gates[211].Set(interrupt_request_handler_211, 0);
    m_gates[212].Set(interrupt_request_handler_212, 0);
    m_gates[213].Set(interrupt_request_handler_213, 0);
    m_gates[214].Set(interrupt_request_handler_214, 0);
    m_gates[215].Set(interrupt_request_handler_215, 0);
    m_gates[216].Set(interrupt_request_handler_216, 0);
    m_gates[217].Set(interrupt_request_handler_217, 0);
    m_gates[218].Set(interrupt_request_handler_218, 0);
    m_gates[219].Set(interrupt_request_handler_219, 0);
    m_gates[220].Set(interrupt_request_handler_220, 0);
    m_gates[221].Set(interrupt_request_handler_221, 0);
    m_gates[222].Set(interrupt_request_handler_222, 0);
    m_gates[223].Set(interrupt_request_handler_223, 0);
    m_gates[224].Set(interrupt_request_handler_224, 0);
    m_gates[225].Set(interrupt_request_handler_225, 0);
    m_gates[226].Set(interrupt_request_handler_226, 0);
    m_gates[227].Set(interrupt_request_handler_227, 0);
    m_gates[228].Set(interrupt_request_handler_228, 0);
    m_gates[229].Set(interrupt_request_handler_229, 0);
    m_gates[230].Set(interrupt_request_handler_230, 0);
    m_gates[231].Set(interrupt_request_handler_231, 0);
    m_gates[232].Set(interrupt_request_handler_232, 0);
    m_gates[233].Set(interrupt_request_handler_233, 0);
    m_gates[234].Set(interrupt_request_handler_234, 0);
    m_gates[235].Set(interrupt_request_handler_235, 0);
    m_gates[236].Set(interrupt_request_handler_236, 0);
    m_gates[237].Set(interrupt_request_handler_237, 0);
    m_gates[238].Set(interrupt_request_handler_238, 0);
    m_gates[239].Set(interrupt_request_handler_239, 0);
    m_gates[240].Set(interrupt_request_handler_240, 0);
    m_gates[241].Set(interrupt_request_handler_241, 0);
    m_gates[242].Set(interrupt_request_handler_242, 0);
    m_gates[243].Set(interrupt_request_handler_243, 0);
    m_gates[244].Set(interrupt_request_handler_244, 0);
    m_gates[245].Set(interrupt_request_handler_245, 0);
    m_gates[246].Set(interrupt_request_handler_246, 0);
    m_gates[247].Set(interrupt_request_handler_247, 0);
    m_gates[248].Set(interrupt_request_handler_248, 0);
    m_gates[249].Set(interrupt_request_handler_249, 0);
    m_gates[250].Set(interrupt_request_handler_250, 0);
    m_gates[251].Set(interrupt_request_handler_251, 0);
    m_gates[252].Set(interrupt_request_handler_252, 0);
    m_gates[253].Set(interrupt_request_handler_253, 0);
    m_gates[254].Set(interrupt_request_handler_254, 0);
    m_gates[255].Set(interrupt_request_handler_255, 0);

    // remap PIC
    out_byte(0x20, 0x11);
    out_byte(0xA0, 0x11);
    out_byte(0x21, 0x20);
    out_byte(0xA1, 0x28);
    out_byte(0x21, 0x04);
    out_byte(0xA1, 0x02);
    out_byte(0x21, 0x01);
    out_byte(0xA1, 0x01);
    out_byte(0x21, 0x0);
    out_byte(0xA1, 0x0);

    load_idt(&m_descriptor);

    x86_64::Sti();

    kprintf("[IDT] IDT initialized. Address=%p size=%u\n", m_descriptor.m_address.Get(), m_descriptor.m_size);
}

} // namespace x86

} // BartOS
