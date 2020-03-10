#include "Runtime/crti.h"

void _init(void)
{
	for ( func_ptr* func = _init_array_start; func != _init_array_end; func++ )
		(*func)();
}

// ---------------------------------------------------------------------------------------------------------

void _fini(void)
{
	for ( func_ptr* func = _fini_array_start; func != _fini_array_end; func++ )
		(*func)();
}

// --------------------------------------------------------------------------------------------------------- 

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
// ---------------------------------------------------------------------------------------------------------

func_ptr _init_array_start[0] __attribute__ ((used, section(".init_array"), aligned(sizeof(func_ptr)))) = { };
func_ptr _fini_array_start[0] __attribute__ ((used, section(".fini_array"), aligned(sizeof(func_ptr)))) = { };
