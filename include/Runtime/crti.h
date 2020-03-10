#ifndef CRTI_H
#define CRTI_H

typedef void (*func_ptr)(void);

// ---------------------------------------------------------------------------------------------------------

/* The ABI requires a 64-bit type.  */
__extension__ typedef int __guard __attribute__((mode(__DI__)));

// ---------------------------------------------------------------------------------------------------------

extern func_ptr _init_array_start[0], _init_array_end[0];
extern func_ptr _fini_array_start[0], _fini_array_end[0];

#endif // CRTI_H