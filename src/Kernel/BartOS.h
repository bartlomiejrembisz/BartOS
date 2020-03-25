#ifndef BARTOS_H
#define BARTOS_H

#include "Libraries/Core/StatusCodes.h"
#include "Kernel/Arch/x86_64/io.h"
#include "Libraries/libc/Kprintf.h"

// Forward declare multiboot_info_t.
struct multiboot_info;
typedef multiboot_info multiboot_info_t;

extern const multiboot_info_t *g_pMultibootInfo;

#endif //! BARTOS_H
