/** 
 * @file 	mos.h
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 05:06:20 pm
 * -----
 * Last Modified: 16 09 2022, 05:19:00 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   mOS - The mini Operating System includes.
 *          Provides all the modules configured in the 
 *          mosconfig file. 
 */
#ifndef mos_include_h
#define mos_include_h
#include <mosconfig.h>

#if MOS_USES(UART)
    #include <uart.h>
#endif

#if MOS_USES(I2C)
    #include <i2c.h>
#endif

#if MOS_USES(SPI)
    #include <spi.h>
#endif

#if MOS_USES(ADC)
    #include <adc.h>
#endif

#if MOS_USES(MD13S)
    #include <md13s.h>
#endif

void setup(void);
void loop(void);

#endif /* mos_include_h */