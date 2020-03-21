#ifndef STD_IO_H
#define STD_IO_H

#include "Kprintf.h"

typedef __builtin_va_list va_list;
typedef __builtin_va_list __isoc_va_list;

#ifdef __cplusplus
extern "C" {
#endif

int putchar(int);

int puts(const char *);

#ifdef __cplusplus
}
#endif

#endif //! STD_IO_H