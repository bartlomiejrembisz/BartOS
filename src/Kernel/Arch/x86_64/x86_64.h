#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  @brief Load the GDT table into GDTR
 *
 *  @param gdt_descriptor pointer to the descriptor object.
 *  @param code_off the code offset into the GDT.
 *  @param data_off the data offset into the GDT
 */
void load_gdt(const void * const gdt_descriptor, const uint64_t code_off, const uint64_t data_off);

/*
 *  @brief Set the code segment.
 *
 *  @param entry_offset the entry offset in the GDT.
 */
void gdt_set_code_segment(const uint64_t entry_offset);

/*
 *  @brief Set the data segment.
 *
 *  @param entry_offset the entry offset in the GDT.
 */
void gdt_set_data_segment(const uint64_t entry_offset);

/*
 *  @brief Load the IDT table into IDTR
 *
 *  @param idt_descriptor pointer to the descriptor object.
 */
void load_idt(const void * const idt_descriptor);

#ifdef __cplusplus
}
#endif
