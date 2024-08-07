/** 
 * @file 	serial.h
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 07:51:28 am
 * -----
 * Last Modified: 18 07 2024, 02:46:04 pm
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
#include <stddef.h>

/**
 * @fn      int serial_init(void);
 * @brief   Initialize the UART peripheral on the MCU.
 * @param   void
 * @return  0 on success, -1 otherwise
 */
int serial_init(void);

/**
 * @fn      int serial_getchar(char *);
 * @brief   Read a character from Serial(UART) module.
 * @param   p pointer to char
 * @return  read char on success, -1 otherwise
 */
int serial_getchar(uint8_t *p);

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

/**
 * @fn      size_t getSerialCount(void);
 * @brief   Get the Serial receive buffer count in bytes
 * @return  Number of bytes pending in serial receive queue.
 */
size_t getSerialCount(void);


#endif /* dev_serial_h */
