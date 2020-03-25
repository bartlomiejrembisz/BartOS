#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void load_gdt(const void * const gdt_descriptor, const uint32_t code_off, const uint32_t data_off);

#ifdef __cplusplus
}
#endif
