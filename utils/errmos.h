/** 
 * @file 	errmos.h
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 08:10:37 am
 * -----
 * Last Modified: 17 09 2022, 12:34:13 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   macros for reporting and retrieving error conditions
 *          using the symbol errmos (short for "error #mini OS")
 */
#ifndef err_mos_h
#define err_mos_h

#define errmos                  (*__errmos())

/**
 * @brief   extern error code var errmos
 *          used by mos libs to set error codes and print function
 *          to diagnose said erros. 
 */
extern int *__errmos(void);

#endif /* err_mos_h */
