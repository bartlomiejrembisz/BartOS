#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void load_gdt(const void * const gdt_descriptor, const uint32_t codeOffset, const uint32_t dataOffset);

#ifdef __cplusplus
}
#endif
