#include "Runtime/crti.h"

func_ptr _init_array_end[0] __attribute__ ((used, section(".init_array"), aligned(sizeof(func_ptr)))) = { };
func_ptr _fini_array_end[0] __attribute__ ((used, section(".fini_array"), aligned(sizeof(func_ptr)))) = { };
