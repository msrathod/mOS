/** 
 * @file 	serial.h
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 07:51:28 am
 * -----
 * Last Modified: 21 09 2022, 10:14:25 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief Serial (UART) driver for mOS
 * 
 */
#ifndef dev_serial_h
#define dev_serial_h
#include <stdint.h>

/**
 * @fn      int serial_init(void);
 * @brief   Initialize the UART peripheral on the MCU.
 * @param   void
 * @return  0 on success, -1 otherwise
 */
int serial_init(void);

/**
 * @fn      int serial_getchar(void);
 * @brief   Read a character from Serial(UART) module.
 * @param   void
 * @return  read char on success, -1 otherwise
 */
int serial_getchar(void);

/**
 * @fn      int serial_putchar(int c);
 * @brief   Write a character to Serial(UART) module.
 * @param   c   character to write 
 * @return  0 on success, -1 otherwise
 */
int serial_putchar(int c);

/**
 * @fn      int serial_write(const char *str);
 * @brief   Write a string literal to Serial(UART) module.
 * @param   str string to write 
 * @return  0 on success, -1 otherwise
 */
int serial_write(const char *str);

/**
 * @fn      size_t getSerialBaud(void);
 * @brief   Get the Serial Baud Rate in hecto bps
 * @return  Baud rate of the serial module in x100 bps or hbps.
 */
size_t getSerialBaud(void);



#endif /* dev_serial_h */
