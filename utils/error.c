/** 
 * @file 	error.c
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 09:57:12 am
 * -----
 * Last Modified: 17 09 2022, 10:31:13 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   errmos implementation
 * 
 */
#include <utils/errmos.h>

static int errnum = 0;


int *__geterrmos(void)
{
    return &errnum;
}