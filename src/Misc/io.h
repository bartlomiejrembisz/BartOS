#ifndef IO_H
#define IO_H

#include <stdint.h>
#include "cpuid.h"

//! Output byte to io port.
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

//! Output word to io port.
static inline void outw(uint16_t port, uint16_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

//! Output dword to io port.
static inline void outl(uint16_t port, uint32_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

//! Read byte from io port
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

//! Read word from io port
static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

//! Read dword from io port
static inline uint32_t inl(uint16_t port)
{
    uint32_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

#endif // IO_H

