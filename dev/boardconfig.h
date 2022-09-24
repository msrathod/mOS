/** 
 * @file 	boardconfig.h
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 10:44:58 pm
 * -----
 * Last Modified: 24 09 2022, 10:34:48 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief 
 * 
 */
#ifndef dev_board_config_h
#define dev_board_config_h

#include <mosconfig.h>
#include <mcu.h>

/* Board LEDs */
#define LED1        BIT0        /* P1.0 */
#define LED1_PORT   P1OUT       /* P1 */
#define LED2        BIT0        /* P1.0 */
#define LED2_PORT   P1OUT       /* P1 */

/* Sensor attached to board pins */
#define SENSOR1     BIT3        /* P1.3 */
#define SENSOR2     BIT4        /* P1.4 */

/* Serial (UART) pin assignment */
#if MOS_USES(UART)
#define UART_RX     BIT1        /* P1.1 */
#define UART_TX     BIT2        /* P1.2 */
#endif /* MOS_USES(UART) */

/* I2C peripheral pin assignment */
#if MOS_USES(I2C)
#define I2C_SCL     BIT6        /* P1.6 */
#define I2C_SDA     BIT7        /* P1.7 */
#endif /* MOS_USES(I2C) */

/* SPI peripheral pin assignment */
#if MOS_USES(SPI)
#define SPI_SCK     BIT5        /* P1.5 */
#define SPI_MISO    BIT6        /* P1.6 */
#define SPI_MOSI    BIT7        /* P1.7 */
#define SPI_CS      BIT0        /* P2.0 */
#endif /* MOS_USES(SPI) */

/* ADC device pin assigment */
#if MOS_USES(ADC)
/* ADC harware pin defs */
#endif /* MOS_USES(ADC) */

/* MD13s device pin assigment */
#if MOS_USES(MD13S)
#define MD13S_PWM   BIT1        /* P2.1 */
#define MD13S_DIR   BIT2        /* P2.2 */
#endif /* MOS_USES(MD13S) */
#endif /* dev_board_config_h */