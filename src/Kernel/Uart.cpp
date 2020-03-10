#include "Kernel/Uart.h"

#include "Misc/MemoryHelper.h"
#include "Core/stdio.h"

namespace ROS
{

namespace
{

inline void delay(int32_t count)
{
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
            : "=r"(count): [count]"0"(count) : "cc");
}

} // namespace

// ---------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

Uart::Uart()
{
    Init();
    puts("Uart constructor!\r\n");
}

// ---------------------------------------------------------------------------------------------------------

void Uart::Init()
{
    MmioWrite<uint32_t>(UART0_CR, 0x00000000);

    MmioWrite<uint32_t>(GPPUD, 0x00000000);
    delay(150);

    MmioWrite<uint32_t>(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    MmioWrite<uint32_t>(GPPUDCLK0, 0x00000000);

    MmioWrite<uint32_t>(UART0_ICR, 0x7FF);

    MmioWrite<uint32_t>(UART0_IBRD, 1);
    MmioWrite<uint32_t>(UART0_FBRD, 40);

    MmioWrite<uint32_t>(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    MmioWrite<uint32_t>(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
        (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    MmioWrite<uint32_t>(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

// ---------------------------------------------------------------------------------------------------------

void Uart::PutByte(const uint8_t byte)
{
    while ( MmioRead<uint32_t>(UART0_FR) & (1 << 5) ) { }
    MmioWrite(UART0_DR, static_cast<uint32_t>(byte));
}

// ---------------------------------------------------------------------------------------------------------

uint8_t Uart::GetByte()
{
    while ( MmioRead<uint32_t>(UART0_FR) & (1 << 4) ) { }
    return MmioRead<uint32_t>(UART0_DR);
}

} // namespace ROS