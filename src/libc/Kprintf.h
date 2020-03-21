#ifndef KPRINT_H
#define KPRINT_H

#include "libc/inttypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

int sprintf(char *buf, const char *fmt, ...);

int kprintf(const char *__restrict, ...);

#ifdef __cplusplus
}
#endif

#endif // KPRINT_H
