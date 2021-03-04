#ifndef BARTOS_H
#define BARTOS_H

#include "Libraries/Core/StatusCodes.h"
#include "Libraries/Misc/Range.h"
#include "Libraries/libc/Kprintf.h"

#include "Kernel/Arch/x86_64/io.h"

#include "frg/macros.hpp"

#include "CoreDefs.h"
#include "DataStructures.h"

#define ASSERT(cond) FRG_ASSERT(cond)

#endif //! BARTOS_H
