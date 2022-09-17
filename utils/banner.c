/** 
 * @file 	banner.c
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 04:44:13 pm
 * -----
 * Last Modified: 17 09 2022, 04:49:55 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief 
 * 
 */
#include <mosprint.h>
#include <utils/banner.h>

void mOSgreet()
{
    OPRINT("\n       _____ _____ ", NULL);
    OPRINT("\n _____|     |   __|", NULL);
    OPRINT("\n|     |  |  |__   |", NULL);
    OPRINT("\n|_|_|_|_____|_____|", NULL);
    MPRINT("\nWelcome to mOS!\t The mini OS for msp430g2xxx. \a\n");
}