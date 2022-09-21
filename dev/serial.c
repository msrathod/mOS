/** 
 * @file 	serial.c
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 04:22:37 pm
 * -----
 * Last Modified: 21 09 2022, 10:35:23 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief Serial (UART) driver for mOS
 * 
 */
#include <mosconfig.h>
#include <stddef.h>
#include <mcu.h>
#include "serial.h"
#include "serialconfig.h"

static const size_t SERIAL_BAUD = MOS_GET(UART_BAUD)/100;

/* Serial Tx only mode omits this. */
#if SERIAL_LITE != 1
#include <utils/queue.h>
extern qid_t serial_qID;
static char serial_qMEM[SERIAL_QUEUE_LEN];
qid_t serial_qID = -1;
#endif /* SERIAL_LITE */

int serial_init()
{
    int ret = -1;
    /* USCI should be in reset before configuring - only configure once */
    if (UCA0CTL1 & UCSWRST) {
        /* Set clock source to SMCLK */
        UCA0CTL1 |= UCSSEL_2;
        UCA0BR0 = UCBR0_UART;
        UCA0BR1 = UCBR1_UART;
        UCA0MCTL = UCBRS_UART;
#if SERIAL_LITE == 1
        /* Enable the USCI peripheral (take it out of reset) */
        UCA0CTL1 &= ~UCSWRST;
        ret = 0;
#else
        q_attr_t attr = {sizeof(serial_qMEM[0]), ARRAY_SIZE(serial_qMEM),
                                serial_qMEM};
        /* Initialize the UART queue */
        if (q_init(&serial_qID, &attr) == 0) 
        {
            /* Enable the USCI peripheral (take it out of reset) */
            UCA0CTL1 &= ~UCSWRST;
            /* Enable rx interrupts */
            IE2 |= UCA0RXIE;
            ret = 0;
        }
#endif/* SERIAL_LITE */
    }
    return ret;
}

int serial_getchar()
{
    int ret = -1;
#if SERIAL_LITE != 1
    char c = -1;
    if (0 == qDequeue(serial_qID, &c)) {    /* dequeue is successful? */
        ret = (int) c;
    } 
#endif/* SERIAL_LITE */
    return ret;
}

int serial_putchar(int c)
{
    /* Wait for the transmit buffer to be ready. */
    while (!(IFG2 & UCA0TXIFG));
    /* Transmit data */
    UCA0TXBUF = (char) c;
    return 0;
}

int serial_write(const char *str)
{
    int ret = -1;
    if (str != NULL) {
        ret = 0;
        while (*str != '\0') {
            /* Wait for the transmit buffer to be ready */
            while (!(IFG2 & UCA0TXIFG));
            /* Transmit data */
            UCA0TXBUF = *str;
            /*  If there is a line-feed, add a carriage return */
            if (*str == '\n') {
                /* Wait for the transmit buffer to be ready */
                while (!(IFG2 & UCA0TXIFG));
                UCA0TXBUF = '\r';
        }
        str++;
        }
    }
    return ret;
}

size_t getSerialBaud()
{
    return SERIAL_BAUD;
}

/* Add ISR if required and not implemented via I2C driver. */
#if (MOS_USES(UART) == 1) && (MOS_USES(I2C) != 2)
#include "serialISR.h"
#endif