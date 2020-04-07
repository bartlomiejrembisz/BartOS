#ifndef IO_H
#define IO_H

#include <stdint.h>
#include "cpuid.h"

//! Output byte to io port.
static inline void out_byte(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "d"(port) );
}

//! Output word to io port.
static inline void out_word(uint16_t port, uint16_t val)
{
    __asm__ volatile ( "outw %0, %1" : : "a"(val), "d"(port) );
}

//! Output dword to io port.
static inline void out_long(uint16_t port, uint32_t val)
{
    __asm__ volatile ( "outl %0, %1" : : "a"(val), "d"(port) );
}

//! Read byte from io port
static inline uint8_t in_byte(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "d"(port) );
    return ret;
}

//! Read word from io port
static inline uint16_t in_word(uint16_t port)
{
    uint16_t ret;
    __asm__ volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "d"(port) );
    return ret;
}

//! Read dword from io port
static inline uint32_t in_long(uint16_t port)
{
    uint32_t ret;
    __asm__ volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "d"(port) );
    return ret;
}

#endif // IO_H

