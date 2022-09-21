/** 
 * @file 	serialconfig.h
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 04:28:01 pm
 * -----
 * Last Modified: 21 09 2022, 05:11:32 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   UART module specific configuration based on options
 *          selected by user in mosconfig. 
 */
#ifndef dev_serial_config_h
#define dev_serial_config_h
#include <mosconfig.h>

#if MOS_USES(UART) == 2
#define SERIAL_LITE             (1)
#else
#define SERIAL_LITE             (0)
#endif

/* If invalid baud configured revert to default value  */
#if (MOS_GET(UART_BAUD)!=115200) && \
    (MOS_GET(UART_BAUD)!=56000) && \
    (MOS_GET(UART_BAUD)!=38400) && \
    (MOS_GET(UART_BAUD)!=19200) && \
    (MOS_GET(UART_BAUD)!=9600)
#define MOS_UART_BAUD       (9600)
#endif

#endif /* dev_serial_config_h */