#include "Runtime/crti.h"

void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
}

// ---------------------------------------------------------------------------------------------------------

int __cxa_guard_acquire (__guard *g) 
{
	return !*(char *)(g);
}

// ---------------------------------------------------------------------------------------------------------

void __cxa_guard_release (__guard *g)
{
	*(char *)g = 1;
}

// ---------------------------------------------------------------------------------------------------------

// Prologue of the _init symbol
__asm__ (
	".section .init;"
	".global _init;"
	".type _init, @function;"
	"_init:;"
	"push %ebp;"
	"movl %esp, %ebp;"
);

// --------------------------------------------------------------------------------------------------------- 

// Prologue of the _fini symbol
__asm__ (
	".section .fini;"
	".global _fini;"
	".type _fini, @function;"
	"_fini:;"
	"push %ebp;"
	"movl %esp, %ebp;"
);
