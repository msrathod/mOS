/** 
 * @file 	board.c
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 10:58:37 pm
 * -----
 * Last Modified: 23 09 2022, 08:56:44 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Board initialization APIs
 * 
 */
#include <mos.h>
#include <dev/clock.h>
#include <dev/tlv.h>
#include <dev/watchdog.h>
#include <utils/errmos.h>
#include "board.h"
#include "boardconfig.h"
#if MOS_USES(UART)
#include <dev/serial.h>
#endif

int board_init()
{
    /* Disable the watchdog module */
    watchdog_disable();

    /* Check TLV for calibration data integrity */
    if (tlv_verify() !=0) {
        while (1);              /* Hang... Calibration data is corrupted */
    }
    /* Initialize the system clocks */
    clock_init();

    /* -------- LED1 setup -------- */
    /* Configure P1.0 as digital output */
    P1SEL &= ~LED1;
    P1DIR |= LED1;
    /* Set P1.0 output high */
    P1OUT |= LED1;

    /* -------- BUTTON setup -------- */
    /* Configure P1.3 to digital input */
    P1SEL &= ~(SENSOR1 + SENSOR2);
    P1SEL2 &= ~(SENSOR1 + SENSOR2);
    P1DIR &= ~(SENSOR1 + SENSOR2);
    /* Pull-up required for sensors */
    P1REN |= (SENSOR1 + SENSOR2);
    P1OUT |= (SENSOR1 + SENSOR2);
    /* Set P1.3 and P1.4 interrupt on high to low edge  */
    P1IES |= (SENSOR1 + SENSOR2);
    /* Enable interrupt on P1.3 and P1.4 */
    P1IE |= (SENSOR1 + SENSOR2);
    P1IFG &= ~(SENSOR1 + SENSOR2);

    /* -------- UART setup -------- */
#if MOS_USES(UART)
    /* Configure P1.1 and P1.2 for UART (USCI_A0) */
    P1SEL |= (UART_RX + UART_TX);
    P1SEL2 |= (UART_RX + UART_TX);
#endif /* MOS_USES(UART) */

    /* -------- I2C setup -------- */
#if MOS_USES(I2C)
    /* Configure P1.6 and P1.7 for I2C */
    P1SEL  |= (I2C_SCL + I2C_SDA);
    P1SEL2 |= (I2C_SCL + I2C_SDA);
#endif /* MOS_USES(I2C) */

    /* -------- MD13S setup -------- */
#if MOS_USES(MD13S)
    /* Configure P2.1 and P2.2 for md13s control */
    P2SEL |= MD13S_PWM;                 // Timer1 TA1.1 on P2.1 (PWM)
    P2SEL &= ~MD13S_DIR;                // GPIO on P2.2 (DIR)
    P2DIR |= (MD13S_PWM + MD13S_DIR);   // pwm and digital output
#endif /* MOS_USES(MD13S) */

    /* Global interrupt enable */
    __enable_interrupt(); 
    
    /* Enable the watchdog module */
    watchdog_enable();

    /* Initialize UART device driver */
#if MOS_USES(UART)
    errmos = serial_init();
    if (errmos != 0) {
        while(1);               /* Hang serial driver failed. */
    }
    EPRINT("\nSerial driver ready");
#endif /* MOS_USES(UART) */
    return 0;
}