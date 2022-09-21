/** 
 * @file 	serialconfig.h
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 04:28:01 pm
 * -----
 * Last Modified: 21 09 2022, 09:39:13 pm
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

/* size of queue for serial driver(must be a power of 2) */
#define SERIAL_QUEUE_LEN        (8)

/* Serial Tx only implementation requested? */
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
#warning "Invalid UART baud, reverting to default baud(9600 bps).
#endif

/* Offset for different freq. obatined by fitting data to 
 * values provided in reference manual (SLAU144)
 */
#if MOS_GET(MCLK_FREQ) == 12
    #define URT      (0)
#elif MOS_GET(MCLK_FREQ) == 1
    #define URT      (3)
#else
    #define URT      (7)
#endif  

#define DIV16   ((MOS_GET(MCLK_FREQ)*1000000) / MOS_GET(UART_BAUD))
#define REM16   ((MOS_GET(MCLK_FREQ)*1000000) % MOS_GET(UART_BAUD))
#define REG16   (((URT*MOS_GET(UART_BAUD))/10) + 8*REM16)/MOS_GET(UART_BAUD)

#define UCBR0_UART          (DIV16 & 0xFF)
#define UCBR1_UART          (DIV16 >> 8)
#define UCBRS_UART          (REG16 << 1)

#endif /* dev_serial_config_h */