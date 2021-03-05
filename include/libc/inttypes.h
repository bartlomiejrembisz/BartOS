#ifndef INTTYPES_H__
#define INTTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/// Division type for maximal integer storage
typedef struct
{
	intmax_t quot; /**< The quotient */
	intmax_t rem; /**< The remainder */
} imaxdiv_t;

/**
 * @brief Computes the absolute value of an intmax_t number.
 *
 * Computes the absolute value of an intmax_t number.
 * The behavior is undefined if the result cannot be represented by the return type.
 *
 * @param j value
 * @return The absolute value of n (i.e. |n|), if it is representable.
 * */
intmax_t imaxabs(intmax_t j);

/**
 * @brief Computes both the quotient and the remainder of the division of the numerator x by the
 * denominator y.
 *
 * Computes both the quotient and the remainder of the division of the numerator x by the
 * denominator y. Computes quotient and remainder simultaneously. The quotient is the algebraic
 * quotient with any fractional part discarded (truncated towards zero). The remainder is such that
 * quot * y + rem == x.
 *
 * Computes the quotient (the result of the expression x/y) and remainder (the result of the
 * expression x%y) simultaneously.
 *
 * @param numer intmax_t values (numerator)
 * @param denom intmax_t values (denominator)
 *
 * @return If both the remainder and the quotient can be represented
 * as objects of the corresponding type (int, long, long long, imaxdiv_t,
 * respectively), returns both as an object of type @see div_t, @see ldiv_t, @see lldiv_t, @see
 * imaxdiv_t.
 *
 * If either the remainder or the quotient cannot be represented, the behavior is undefined.
 * */
imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);

#if UINTPTR_MAX == UINT64_MAX
#define __PRI64 "l"
#define __PRIPTR "l"
#else
#define __PRI64 "ll"
#define __PRIPTR ""
#endif

#define PRId8 "hhd"
#define PRId16 "hd"
#define PRId32 "d"
#define PRId64 __PRI64 "d"

#define PRIdLEAST8 "hhd"
#define PRIdLEAST16 "hd"
#define PRIdLEAST32 "d"
#define PRIdLEAST64 __PRI64 "d"

#define PRIdFAST8 "hhd"
#define PRIdFAST16 "d"
#define PRIdFAST32 "d"
#define PRIdFAST64 __PRI64 "d"

#define PRIi8 "hhi"
#define PRIi16 "hi"
#define PRIi32 "i"
#define PRIi64 __PRI64 "i"

#define PRIiLEAST8 "hhi"
#define PRIiLEAST16 "h"
#define PRIiLEAST32 "i"
#define PRIiLEAST64 __PRI64 "i"

#define PRIiFAST8 "hhi"
#define PRIiFAST16 "i"
#define PRIiFAST32 "i"
#define PRIiFAST64 __PRI64 "i"

#define PRIo8 "hho"
#define PRIo16 "ho"
#define PRIo32 "o"
#define PRIo64 __PRI64 "o"

#define PRIoLEAST8 "hho"
#define PRIoLEAST16 "ho"
#define PRIoLEAST32 "o"
#define PRIoLEAST64 __PRI64 "o"

#define PRIoFAST8 "hho"
#define PRIoFAST16 "o"
#define PRIoFAST32 "o"
#define PRIoFAST64 __PRI64 "o"

#define PRIu8 "hhu"
#define PRIu16 "hu"
#define PRIu32 "u"
#define PRIu64 __PRI64 "u"

#define PRIuLEAST8 "hhu"
#define PRIuLEAST16 "hu"
#define PRIuLEAST32 "u"
#define PRIuLEAST64 __PRI64 "u"

#define PRIuFAST8 "hhu"
#define PRIuFAST16 "u"
#define PRIuFAST32 "u"
#define PRIuFAST64 __PRI64 "u"

#define PRIx8 "hhx"
#define PRIx16 "hx"
#define PRIx32 "x"
#define PRIx64 __PRI64 "x"

#define PRIxLEAST8 "hhx"
#define PRIxLEAST16 "hx"
#define PRIxLEAST32 "x"
#define PRIxLEAST64 __PRI64 "x"

#define PRIxFAST8 "hhx"
#define PRIxFAST16 "x"
#define PRIxFAST32 "x"
#define PRIxFAST64 __PRI64 "x"

#define PRIX8 "hhX"
#define PRIX16 "hX"
#define PRIX32 "X"
#define PRIX64 __PRI64 "X"

#define PRIXLEAST8 "hhX"
#define PRIXLEAST16 "hX"
#define PRIXLEAST32 "X"
#define PRIXLEAST64 __PRI64 "X"

#define PRIXFAST8 "hhX"
#define PRIXFAST16 "X"
#define PRIXFAST32 "X"
#define PRIXFAST64 __PRI64 "X"

#define PRIdMAX __PRI64 "d"
#define PRIiMAX __PRI64 "i"
#define PRIoMAX __PRI64 "o"
#define PRIuMAX __PRI64 "u"
#define PRIxMAX __PRI64 "x"
#define PRIXMAX __PRI64 "X"

#define PRIdPTR __PRIPTR "d"
#define PRIiPTR __PRIPTR "i"
#define PRIoPTR __PRIPTR "o"
#define PRIuPTR __PRIPTR "u"
#define PRIxPTR __PRIPTR "x"
#define PRIXPTR __PRIPTR "X"

#ifdef __cplusplus
}
#endif

#endif // INTTYPES_H__
