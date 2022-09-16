/** 
 * @file 	moslib.h
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 05:46:04 pm
 * -----
 * Last Modified: 16 09 2022, 09:43:02 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   mOS routine library. 
 */
#ifndef mos_lib_h
#define mos_lib_h
#include <stdint.h>

/**
 * @fn      char* itohex(int n, char* buf) 
 * @brief   Converts integer n as an unsigned hexadecimal constant 
 *          character  string.  The string is placed in the buffer 
 *          passed,  which must be large enough to hold the output. 
 * @param   n       integer to convert
 * @param   buf     pointer to buffer to hold the string.
 * 
 * @return  char*   buffer containing the null terminated string.
 */
char* itohex(int n, char* buf);

/**
 * @fn      char* itoasc(uint16_t n, char* buf)
 * @brief   Converts unsigned integer n into a ascii character string.
 *          The string is placed in the buffer passed,  which must be
 *          large enough to hold the output.
 * @param   n       integer to convert 
 * @param   buf     pointer to buffer to hold the converted string.
 * @return  char*   buffer containing the null terminated string.
 */
char* itoasc(uint16_t n, char* buf);

#endif /* mos_lib_h */