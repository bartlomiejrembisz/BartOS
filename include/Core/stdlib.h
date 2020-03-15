#ifndef STD_LIB_H
#define STD_LIB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_STDLIB_MALLOC
/*
 *  @brief Copy bytes from one buffer to another
 * 
 *  @param  pDestination the buffer to copy to.
 *  @param  pSource the buffer to copy from.
 *  @param  nBytesToCopy the amount of bytes to copy.
 */
void *memcpy(void *pDestination, void *pSource, const size_t nBytesToCopy);
#endif

/*
 *  @brief Erase specifiec bytes from a buffer
 * 
 *  @param pBuffer the buffer to clear.
 *  @param nBytesToClear the amount of bytes to clear.
 */
void bzero(void *pBuffer, const size_t nBytesToClear);

#ifdef __cplusplus
}
#endif

#endif //! STD_LIB_H