/** 
 * @file 	clockconfig.h
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 04:56:34 pm
 * -----
 * Last Modified: 21 09 2022, 05:11:26 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Clock module specific configurations based on options
 *          set by user in mosconfig.
 */
#ifndef dev_clock_config_h
#define dev_clock_config_h
#include <mcu.h>
#include "clock.h"

/* Main Clock reg settings */
#if MOS_GET(MCLK_FREQ) == 16
    #define CLK_BC1_REG         CALBC1_16MHZ
    #define CLK_DCO_REG         CALDCO_16MHZ
#elif MOS_GET(MCLK_FREQ) == 12
    #define CLK_BC1_REG         CALBC1_12MHZ
    #define CLK_DCO_REG         CALDCO_12MHZ
#elif MOS_GET(MCLK_FREQ) == 8
    #define CLK_BC1_REG         CALBC1_8MHZ
    #define CLK_DCO_REG         CALDCO_8MHZ
#elif MOS_GET(MCLK_FREQ) == 1
    #define CLK_BC1_REG         CALBC1_1MHZ
    #define CLK_DCO_REG         CALDCO_1MHZ
#else
#error "Clock configuration invalid."
#endif /* Main clock settings */

/* Aux. clock reg settings */
#if MOS_GET(ACLK_FREQ) == 32768
    #define CLK_BC3_REG         (LFXT1S_0 + XCAP_1) 
#else
    #define CLK_BC3_REG         LFXT1S_2
#endif /* Aux clock settings */

#endif /* dev_clock_config_h */