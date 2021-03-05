/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#include <strings.h>

#ifdef I_AM_QSORT_R
typedef int cmp_t(void*, const void*, const void*);
#else
typedef int cmp_t(const void*, const void*);
#endif
static inline char* med3(char* a, char* b, char* c, cmp_t* cmd, void* thunk)
	__attribute__((always_inline));
static inline void swapfunc(char* a, char* b, size_t n, size_t swaptype)
	__attribute__((always_inline));

#define min(a, b) (a) < (b) ? a : b

/*
 * Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
 */
#define swapcode(TYPE, parmi, parmj, n) \
	{                                   \
		size_t i = (n) / sizeof(TYPE);  \
		TYPE* pi = (TYPE*)(parmi);      \
		TYPE* pj = (TYPE*)(parmj);      \
		do                              \
		{                               \
			TYPE t = *pi;               \
			*pi++ = *pj;                \
			*pj++ = t;                  \
		} while(--i > 0);               \
	}

#define SWAPINIT(a, es) \
	swaptype = (uintptr_t)a % sizeof(long) || es % sizeof(long) ? 2 : es == sizeof(long) ? 0 : 1;

static inline void swapfunc(char* a, char* b, size_t n, size_t swaptype)
{
	if(swaptype == 0)
	{
		long t = *(long*)(void*)(a);
		*(long*)(void*)(a) = *(long*)(void*)(b);
		*(long*)(void*)(b) = t;
	}
	else if(swaptype == 1)
	{
		swapcode(long, (void*)a, (void*)b, n)
	}
	else
	{
		swapcode(char, a, b, n)
	}
}

#define swap(a, b) swapfunc(a, b, (size_t)es, (size_t)swaptype)

// this switch exists because we needed to clean up
// the swap() macro, and vecswap has a different 0 meaning.
// To force the right swapfunc behavior we force it to 1 if it's
#define vecswap(a, b, n) \
	if((n) > 0)          \
	swapfunc(a, b, n, swaptype ? (size_t)swaptype : 1)

#ifdef I_AM_QSORT_R
#define CMP(t, x, y) (cmp((t), (x), (y)))
#else
#define CMP(t, x, y) (cmp((x), (y)))
#endif

static inline char* med3(char* a, char* b, char* c, cmp_t* cmp,
						 void* thunk
#ifndef I_AM_QSORT_R
						 __attribute__((unused))
#endif
)
{
	return CMP(thunk, a, b) < 0 ? (CMP(thunk, b, c) < 0 ? b : (CMP(thunk, a, c) < 0 ? c : a))
								: (CMP(thunk, b, c) > 0 ? b : (CMP(thunk, a, c) < 0 ? a : c));
}

#ifdef __LP64__
#define DEPTH(x) (2 * (flsl((long)(x)) - 1))
#else /* !__LP64__ */
#define DEPTH(x) (2 * (fls((int)(x)) - 1))
#endif /* __LP64__ */

static void _qsort(void* a, size_t n, size_t es,
#ifdef I_AM_QSORT_R
				   void* thunk,
#else
#define thunk NULL
#endif
				   cmp_t* cmp, int depth_limit)
{
	char* pa;
	char* pb;
	char* pc;
	char* pd;
	char* pl;
	char* pm;
	char* pn;
	size_t d;
	size_t r;
	size_t swap_cnt;
	int cmp_result;
	int swaptype;

loop:
	if(depth_limit-- <= 0)
	{
#ifdef I_AM_QSORT_R
		heapsort_r(a, n, es, thunk, cmp);
#else
		heapsort(a, n, es, cmp);
#endif
		return;
	}
	SWAPINIT(a, es);
	swap_cnt = 0;
	if(n < 7)
	{
		for(pm = (char*)a + es; pm < (char*)a + n * es; pm += es)
		{
			for(pl = pm; pl > (char*)a && CMP(thunk, pl - es, pl) > 0; pl -= es)
			{
				swap(pl, pl - es);
			}
		}
		return;
	}
	pm = (char*)a + (n / 2) * es;
	if(n > 7)
	{
		pl = a;
		pn = (char*)a + (n - 1) * es;
		if(n > 40)
		{
			d = (n / 8) * es;
			pl = med3(pl, pl + d, pl + 2 * d, cmp, thunk);
			pm = med3(pm - d, pm, pm + d, cmp, thunk);
			pn = med3(pn - 2 * d, pn - d, pn, cmp, thunk);
		}
		pm = med3(pl, pm, pn, cmp, thunk);
	}
	swap(a, (void*)pm);
	pa = pb = (char*)a + es;

	pc = pd = (char*)a + (n - 1) * es;
	for(;;)
	{
		while(pb <= pc && (cmp_result = CMP(thunk, pb, a)) <= 0)
		{
			if(cmp_result == 0)
			{
				swap_cnt = 1;
				swap(pa, pb);
				pa += es;
			}
			pb += es;
		}
		while(pb <= pc && (cmp_result = CMP(thunk, pc, a)) >= 0)
		{
			if(cmp_result == 0)
			{
				swap_cnt = 1;
				swap(pc, pd);
				pd -= es;
			}
			pc -= es;
		}
		if(pb > pc)
		{
			break;
		}
		swap(pb, pc);
		swap_cnt = 1;
		pb += es;
		pc -= es;
	}

	pn = (char*)a + n * es;
	r = min((uintptr_t)pa - (uintptr_t)a, (uintptr_t)pb - (uintptr_t)pa);
	vecswap(a, pb - r, r);
	r = min((uintptr_t)pd - (uintptr_t)pc, (uintptr_t)pn - (uintptr_t)pd - (uintptr_t)es);
	vecswap(pb, pn - r, r);

	if(swap_cnt == 0)
	{ /* Switch to insertion sort */
		r = 1 + n / 4; /* n >= 7, so r >= 2 */
		for(pm = (char*)a + es; pm < (char*)a + n * es; pm += es)
		{
			for(pl = pm; pl > (char*)a && CMP(thunk, pl - es, pl) > 0; pl -= es)
			{
				swap(pl, pl - es);
				if(++swap_cnt > r)
				{
					goto nevermind;
				}
			}
		}
		return;
	}

nevermind:
	if((r = (uintptr_t)pb - (uintptr_t)pa) > es)
	{
#ifdef I_AM_QSORT_R
		_qsort(a, r / es, es, thunk, cmp, depth_limit);
#else
		_qsort(a, r / es, es, cmp, depth_limit);
#endif
	}
	if((r = (uintptr_t)pd - (uintptr_t)pc) > es)
	{
		/* Iterate rather than recurse to save stack space */
		a = pn - r;
		n = r / es;
		goto loop;
	}
	/*		qsort(pn - r, r / es, es, cmp);*/
}

void
#ifdef I_AM_QSORT_R
qsort_r(void *a, size_t n, size_t es, void *thunk, cmp_t *cmp)
#else
qsort(void *a, size_t n, size_t es, cmp_t *cmp)
#endif
{
	_qsort(a, n, es,
#ifdef I_AM_QSORT_R
		   thunk,
#endif
		   cmp, DEPTH(n));
}
