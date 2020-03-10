#ifndef STD_IO_H
#define STD_IO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  @brief Get char from I/O
 */
char getc(void);

/*
 *  @brief Put char to I/O
 * 
 *  @param  c the character.
 */
void putc(char c);

/*
 *  @brief Put string to I/O
 * 
 *  @param  pStr buffer containing the string.
 */
void puts(const char *pStr);

/*
 *  @brief Get string from I/O
 * 
 *  @param  pBuffer buffer to copy the string into.
 *  @param  bufferLength the length of the buffer.
 */
void gets(char *pBuffer, int bufferLength);

#ifdef __cplusplus
}
#endif

#endif //! STD_IO_H