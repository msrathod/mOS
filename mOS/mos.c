/** 
 * @file 	mos.c
 * @author 	Mohit Rathod
 * Created: 24 09 2022, 08:42:04 am
 * -----
 * Last Modified: 24 09 2022, 10:37:50 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   mOS System APIs
 * 
 */
#include <mos.h>
#include <mcu.h>
#include <dev/board.h>
#include <dev/clock.h>
#include <dev/sysled.h>
#include <dev/watchdog.h>
#include <utils/banner.h>

int main(void)
{
    if (board_init() == 0) {
        mossAddTask(sysled_TOGGLE, 10, 50);
        setup();
        mOSgreet();
        while (1) {
            watchdog_pet();
            mossRun();
            loop();
        }
    }
    return 0;
}
