/** 
 * @file 	i2cmaster.c
 * @author 	Mohit Rathod
 * Created: 21 07 2024, 05:27:13 pm
 * -----
 * Last Modified: 22 07 2024, 08:46:31 am
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C Master driver api
 * 
 */
#include <dev/i2cmaster.h>
#include <mosconfig.h>
#include <mcu.h>

#define I2C_PRESCALER       ((MOS_GET(MCLK_FREQ)*1000000)/MOS_GET(I2C_SPEED))

static int _transmit(const uint8_t *buf, size_t nbytes);
static int _receive(uint8_t *buf, size_t nbytes);
static int _check_ack();

/**
 * \brief Initialize the I2C peripheral
 * \return 0 on success, -1 otherwise
 */
int i2cmaster_init(void)
{
    UCB0CTL1 |= UCSWRST;                        // Enable SW reset
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;       // I2C Master, synchronous mode
    UCB0BR0 = (I2C_PRESCALER);                  // Configure the baud rate 
    UCB0BR1 = 0;                                // registers for 100kHz when
                                                // sourcing from SMCLK
    UCB0CTL1 = UCSSEL_2;                        // Clear SW rst & select SMCLK
    return 0;
}

/**
 * \brief Perform an I2C transfer
 * \param[in] dev - the I2C slave device
 * \param[in/out] data - data structure containing the buffers
 * \return 0 on success, -1 otherwise
 */
int i2cmaster_transfer(const struct i2c_device *dev, struct i2c_data *data)
{
    int err = 0;
    /* Set the slave device address */
    UCB0I2CSA = dev->address;
 
    /* Transmit data is there is any */
    if (data->tx_len > 0) {
        err = _transmit((const uint8_t *) data->tx_buf, data->tx_len);
    }

    /* Receive data is there is any */
    if ((err == 0) && (data->rx_len > 0)) {
        err = _receive((uint8_t *) data->rx_buf, data->rx_len);
    } else {
        /* No bytes to receive send the stop condition */
        UCB0CTL1 |= UCTXSTP; 
    }
    return err;
}

/**
 * \brief Check for ACK/NACK and handle NACK condition if occured
 * \return 0 if slave ACK'd, -1 if slave NACK'd
 */
static int _check_ack()
{
    int err = 0;
    if(UCB0STAT & UCNACKIFG)                /* check for NACK */
    {    
        UCB0CTL1 |= UCTXSTP;                /* Stop the i2c transmission */
        UCB0STAT &= ~UCNACKIFG;             /* Clear the nack flag */
        err = -1;
    }
    return err;
}

/**
 * \brief Transmit data to the slave device
 * \param[in] buf - the buffer of data to transmit
 * \param[in] nbytes - the number of bytes to transmit
 * \return 0 on success, -1 otherwise
 */
static int _transmit(const uint8_t *buf, size_t nbytes)
{
    int err = 0;
    UCB0CTL1 |= UCTR | UCTXSTT;                 /* Send the start condition */
    /* Wait for the start condition to be sent and ready to tx interrupt */
    while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));
    err = _check_ack();                         /* Check for ACK */
    while ((err == 0) && (nbytes > 0)) 
    {                                           /* If no error & bytes left  */ 
        UCB0TXBUF = *buf;                       /* transmit the data*/
        while ((IFG2 & UCB0TXIFG) == 0) 
        {
            err = _check_ack();
            if (err < 0){
                break;
            }
        }
        buf++;
        nbytes--;
    }
    return err;
}

/**
 * \brief Receive data from the slave device
 * \param[in] buf - the buffer to store the received data
 * \param[in] nbytes - the number of bytes to receive
 * \return 0 on success, -1 otherwise
 */
static int _receive(uint8_t *buf, size_t nbytes)
{
    int err = 0;
    UCB0CTL1 &= ~UCTR;                  /* Send the start and wait */
    UCB0CTL1 |= UCTXSTT;
    while(UCB0CTL1 & UCTXSTT);          /* Wait for the START to be sent */
    
    if (nbytes == 1) {                  /* If there is only one byte to rx */
        UCB0CTL1 |= UCTXSTP;            /* send STP after STT has been sent */
    }
    err = _check_ack();                 /* Check for ACK */

    while((err == 0) && (nbytes > 0))   /* If no error & bytes left to rx */
    {                                   
        while((IFG2 & UCB0RXIFG) == 0); /* Wait for the data */

        *buf = UCB0RXBUF;               /* Rx the data */
        buf++;
        nbytes--;

        if (nbytes == 1) {              /* If there is only 1 byte left to rx */
            UCB0CTL1 |= UCTXSTP;        /* send STP condition */
        }
    }
    return err;
}
