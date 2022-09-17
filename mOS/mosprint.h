/** 
 * @file 	mosprint.h
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 09:51:59 pm
 * -----
 * Last Modified: 17 09 2022, 04:46:51 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   mOS printing and debug routine library. This library
 *          provides methods to display messages via serial terminal
 *          to a host. Useful for debuging and greeting.
 */
#ifndef mos_print_h
#define mos_print_h
#include <mosconfig.h>
#include <stddef.h>

/* select print functionality based on verbosity set in mosconfig */
#if MOS_GET(VERBOSITY) == 3
#define MPRINT(x)               oprint((x), NULL)
#define OPRINT(x, y)            oprint((x), (y))
#define EPRINT(x)               eprint(x)
#elif MOS_GET(VERBOSITY) == 2
#define MPRINT(x)               oprint((x), NULL)
#define OPRINT(x, y)            oprint((x), (y))
#define EPRINT(x)               do { if (0) eprint(x); } while (0)
#elif MOS_GET(VERBOSITY) == 1
#define MPRINT(x)               oprint((x), NULL)
#define OPRINT(x, y)            do { if (0) oprint((x), (y)); } while(0)
#define EPRINT(x)               do { if (0) eprint(x); } while (0)
#else
#define MPRINT(x)               do { if (0) oprint((x), NULL); } while(0)
#define OPRINT(x, y)            do { if (0) oprint((x), (y)); } while(0)
#define EPRINT(x)               do { if (0) eprint(x); } while (0)
#endif /* VERBOSITY */

/**
 * @enum    ioEnum 
 * @brief   IO channel enumeration. Helps in deciding the medium
 *          used to transfer a char string for printing.
 */
typedef enum ioEnum
{
    CHNL_ERR,
    CHNL_UART,
    CHNL_LCD,
    CHNL_MAX
}iochnl_t;

/**
 * @fn      void eprint(const char* str);
 * @brief   Error print. Debug helper function that prints the
 *          const string literal followed by a message derived
 *          from the extern error code errmos. errmos is print
 *          as a hexadecimal constant.
 * @param   str     const string message to display before errmos
 * @return  void
 * @note    This fn isn't called directly.
 *          use the EPRINT(x) macro fn instead.
 */
void eprint(const char* str);

/**
 * @fn      void oprint(const char* str, int *ptr);
 * @brief   OS print. Diagnostic and OS information print helper.
 *          Displays the string literal message followed by int 
 *          value passed, which should be unsigned. To print a 
 *          string alone pass a NULL pointer to the int. 
 * @param   str     const string literal to print. 
 * @param   ptr     pointer to integer to print.
 * @return  void 
 */
void oprint(const char* str, int *ptr);

/**
 * @fn      void print(const char* str, iomchnl iochnl);
 * @brief   Main os print function that redirects the message to 
 *          appropriate IO channel based on the 'iochnl' param. 
 * @todo    Implement support for LCD_puts().
 * @param   str     const string literal to print.
 * @param   iochnl  IO channel used to print 'str'.
 * @return  void 
 */
void print(const char* str, iochnl_t iochnl);

#endif /* mos_print_h */
