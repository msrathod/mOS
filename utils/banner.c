/** 
 * @file 	banner.c
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 04:44:13 pm
 * -----
 * Last Modified: 24 09 2022, 03:47:38 pm
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
#include <dev/clock.h>
#if MOS_USES(UART)
#include <dev/serial.h>
#endif
#ifdef MOS_USES(MD13S)
#include <dev/md13s.h>
#endif

void mOSgreet()
{
    int tmp;
    OPRINT("\n       _____ _____ ", NULL);
    OPRINT("\n _____|     |   __|", NULL);
    OPRINT("\n|     |  |  |__   |", NULL);
    OPRINT("\n|_|_|_|_____|_____|", NULL);
    MPRINT("\nWelcome to mOS!\t The mini OS for msp430g2xxx. \a\n");
    tmp = getMCLK();
    OPRINT("\nClock [Main]:\t", &tmp);OPRINT(" MHz", NULL);
    tmp = getACLK();
    OPRINT("\nClock [Aux]:\t", &tmp);OPRINT(" Hz", NULL);
#if MOS_USES(UART)
    tmp = getSerialBaud();
    OPRINT("\nBaud rate:\t", &tmp);OPRINT("00 bps", NULL);
#endif
#ifdef MOS_USES(MD13S)
    tmp = getMD13sFreq();
    OPRINT("\nMD13s PWM freq:\t", &tmp);OPRINT(" Hz", NULL);
#endif
}