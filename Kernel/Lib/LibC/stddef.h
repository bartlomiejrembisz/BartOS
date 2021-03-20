#ifndef STDDEF_H_
#define STDDEF_H_

#include <_types/_ptrdiff_t.h>
#include <_types/_size_t.h>

#ifdef __cplusplus
/*! Expands to 0L (for C++). Implementation-defined null pointer constant  */
#define NULL 0L
#else

/*! Expands to ((void*)0) (not C++). Implementation-defined null pointer constant  */
#define NULL ((void*)0)
#endif

#if __GNUC__ > 3

/*! byte offset from the beginning of a struct type to specified member  */
#define offsetof(type, member) __builtin_offsetof(type, member)
#else

/*! byte offset from the beginning of a struct type to specified member  */
#define offsetof(type, member) ((size_t)((char*)&(((type*)0)->member) - (char*)0))
#endif

#if(defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) || \
	(defined(__cplusplus) && __cplusplus >= 201103L)

#include_next <stddef.h>
#endif

#endif // STDDEF_H_
