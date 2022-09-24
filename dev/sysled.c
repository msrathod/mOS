/** 
 * @file 	sysled.c
 * @author 	Mohit Rathod
 * Created: 24 09 2022, 10:19:59 pm
 * -----
 * Last Modified: 24 09 2022, 10:36:29 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   System LED control APIs
 * 
 */
#include <mcu.h>
#include <dev/boardconfig.h>
#define LEDPORT(x)              x ## _PORT
void sysled_ON()
{
    LEDPORT(LED1) |= LED1;
}

void sysled_OFF()
{
    LEDPORT(LED1) &= ~LED1;
}

void sysled_TOGGLE()
{
    LEDPORT(LED1) ^= LED1;
}