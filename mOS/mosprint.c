/** 
 * @file 	mosprint.c
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 07:47:28 am
 * -----
 * Last Modified: 17 09 2022, 04:46:58 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   mOS printing APIs.
 */
#include <mosprint.h>
#include <moslib.h>
#include <dev/serial.h>
#include <utils/errmos.h>

/* private vars */
/* A large enough buffer that can hold an int as ascii */
static char _strn[6];

/* IO channel selection */
#if MOS_GET(STDIO_CHNL) == 2
#define OS_printR(x)              lcd_write(x)
#else
#define OS_printR(x)              serial_write(x)
#endif

void print(const char* str, iochnl_t iochnl)
{
    if (CHNL_UART == iochnl) {
        serial_write(str);
    } else if (CHNL_LCD == iochnl) {
        /* lcd_write(str); */
    }
}

void oprint(const char* str, int *ptr)
{
    OS_printR(str);
    if (ptr != NULL) {
        itoasc(*ptr, _strn);
        OS_printR(_strn);
    }
}

void eprint(const char* str)
{
    OS_printR(_strn);
    if (errmos != 0) {
        OS_printR(" FAILURE [0x");
        itohex(errmos, _strn);
        OS_printR(_strn);
        OS_printR("]");
        errmos = 0;
    } else {
        OS_printR("[SUCCESS]");
    }
}