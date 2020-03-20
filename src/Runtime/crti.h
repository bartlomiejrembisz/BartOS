#ifndef CRTI_H
#define CRTI_H

typedef void (*func_ptr)(void);

// ---------------------------------------------------------------------------------------------------------

/* The ABI requires a 64-bit type.  */
__extension__ typedef int __guard __attribute__((mode(__DI__)));

// ---------------------------------------------------------------------------------------------------------

#endif // CRTI_H