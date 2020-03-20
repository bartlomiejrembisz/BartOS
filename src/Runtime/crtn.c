#include "crti.h"

// Epilogue of the _init symbol
__asm__ (
	".section .init;"
	"popl %ebp;"
	"ret"
);

// --------------------------------------------------------------------------------------------------------- 

// Epilogue of the _fini symbol
__asm__ (
	".section .fini;"
	"popl %ebp;"
	"ret"
);
