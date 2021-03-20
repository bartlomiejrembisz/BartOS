#ifndef VM_H
#define VM_H

#include "BartOS.h"

namespace BartOS
{

namespace MM
{

//! VM area types.
enum class VmAreaType
{
    UNDEFINED,
    EXE_SECTION,
    PERMANENT,
    HEAP,
    ANONYMOUS,
    STACK,
    TEMP_MAPPING
};

//! Properties of the VmArea
enum class VmAreaFlags
{
    NO_FLAGS        = 0 << 0,
    EXECUTABLE      = 1 << 1,   ///< Executable file.
    HUGE_PAGES      = 1 << 2,   ///< Whether area uses huge pages only.
    CAN_GROW        = 1 << 3,   ///< Whether the area can grow.
    GROWS_DOWN      = 1 << 4,   ///< Whether the area grows down.
    IS_CONTIGUOUS   = 1 << 5,   ///< Whether the area is physically contiguous.
};

} // namespace MM

} // namespace BartOS

#endif // VM_H