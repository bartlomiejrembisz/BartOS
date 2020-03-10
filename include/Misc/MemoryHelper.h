#ifndef MEMORY_HELPER_H
#define MEMORY_HELPER_H

#include <stdint.h>

namespace ROS
{

typedef uint32_t AddressWidth;

template <typename DATA_TYPE, typename ADDRESS_TYPE>
inline void MmioWrite(const ADDRESS_TYPE address, DATA_TYPE data)
{
    *(volatile DATA_TYPE *) address = data;
}

// ---------------------------------------------------------------------------------------------------------

template <typename DATA_TYPE, typename ADDRESS_TYPE>
inline DATA_TYPE MmioRead(const ADDRESS_TYPE address)
{
    return *(volatile DATA_TYPE *) address;
}

} // namespace ROS

#endif //! MEMORY_HELPER_H