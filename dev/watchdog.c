/** 
 * @file 	watchdog.c
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 08:29:40 pm
 * -----
 * Last Modified: 23 09 2022, 08:34:09 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Watchdog APIs
 * 
 */
#include <mcu.h>
#include "watchdog.h"

void watchdog_disable()
{
    /* Hold the watchdog */
    WDTCTL = WDTPW + WDTHOLD;
}

void watchdog_enable()
{
    /* Read the watchdog interrupt flag */
    if (IFG1 & WDTIFG) {
        /* clear the flag */
        IFG1 &= ~WDTIFG;
    }
    watchdog_pet();
}

void watchdog_pet()
{
    /**
     * Enable the watchdog with following settings
     *   - sourced by ACLK
     *   - interval = 32786 / 12000 = 2.73s
     */ 
    WDTCTL = WDTPW + (WDTSSEL | WDTCNTCL);
}