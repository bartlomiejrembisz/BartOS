#include "Core/stdlib.h"

#ifdef USE_STDLIB_MALLOC
void *memcpy(void *pDestination, void *pSource, const size_t nBytesToCopy)
{
    char *pCurrentSource = (char *) pSource;
    char *pCurrentDestination = (char *) pDestination;
    
    for (size_t nBytesCopied = 0; nBytesCopied < nBytesToCopy; ++nBytesCopied)
        pCurrentDestination[nBytesCopied] = pCurrentSource[nBytesCopied]; 


    return pDestination;
}
#endif

// ---------------------------------------------------------------------------------------------------------

void bzero(void *pBuffer, const size_t nBytesToClear)
{
    const char * const pEnd = ((const char * const) pBuffer) + nBytesToClear;
    char * pCurrent = (char *) pBuffer;
    while (pEnd != pCurrent)
    {
        pCurrent = 0;
        ++pCurrent;
    }
}
