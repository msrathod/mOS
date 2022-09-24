/** 
 * @file 	moslib.c
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 09:12:18 pm
 * -----
 * Last Modified: 24 09 2022, 08:12:02 am
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief mOS standard routine library.
 * 
 */
#include <moslib.h>

/**
 * @fn      static char* _reverse_in_place(char* str, int len)
 * @brief   Reverses the order of character in the string passed upto
 *          length 'len'.
 * @param   str     pointer to string.
 * @param   len     length of string to reverse.
 * @return  char*   buffer containing the reversed string.
 */
static char* _reverse_in_place(char* str, int len);

char* itohex(int n, char* buf)
{
    int i = 0;
    do {
        int r = n & 15; /* get remainder eq. to n%16 */
        if (r >= 10) {
            buf[i++] = 'A' + (r - 10);
        }else{
            buf[i++] = '0' + r;
        }
        n = n>>4;       /* get quotient eq. to n/16 */
    }while (n);
    buf[i] = '\0';      /* null terminate string */
    return _reverse_in_place(buf, i);
}

char* itoasc(uint16_t n, char* buf)
{
    int i = 0;
    do {
        int r = n % 10;         /* this digit */
        buf[i++] = '0' + r;
        n = n/10;               /* shift digit */
    }while (n);

    buf[i] = '\0';              /* null terminate string */
    return _reverse_in_place(buf, i);
}

static char* _reverse_in_place(char* str, int len)
{
    char* p1 = str;
    char* p2 = str + len - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1++ = *p2;
        *p2-- = tmp;
    }
    return str;
}