/** 
 * @file 	i2cmaster.c
 * @author 	Mohit Rathod
 * Created: 21 07 2024, 05:27:13 pm
 * -----
 * Last Modified: 21 07 2024, 06:00:10 pm
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

static int _transmit(const struct i2c_device *dev, const uint8_t *buf, size_t nbytes);
static int _receive(const struct i2c_device *dev, uint8_t *buf, size_t nbytes);
static int _check_ack(const struct i2c_device *dev);

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
int i2c_transfer(const struct i2c_device *dev, struct i2c_data *data)
{
    int err = 0;
    /* Set the slave device address */
    UCB0I2CSA = dev->address;
 
    /* Transmit data is there is any */
    if (data->tx_len > 0) {
        err = _transmit(dev, (const uint8_t *) data->tx_buf, data->tx_len);
    }

    /* Receive data is there is any */
    if ((err == 0) && (data->rx_len > 0)) {
        err = _receive(dev, (uint8_t *) data->rx_buf, data->rx_len);
    } else {
        /* No bytes to receive send the stop condition */
        UCB0CTL1 |= UCTXSTP; 
    }
            
    return err;
}

/**
 * \brief Check for ACK/NACK and handle NACK condition if occured
 * \param[in] dev - the I2C slave device
 * \return 0 if slave ACK'd, -1 if slave NACK'd
 */
static int _check_ack(const struct i2c_device *dev)
{
    int err = 0;
    IGNORE(dev);
 
    /* Check for ACK */
    if (UCB0STAT & UCNACKIFG) {    
        /* Stop the I2C transmission */
        UCB0CTL1 |= UCTXSTP;

        /* Clear the interrupt flag */
        UCB0STAT &= ~UCNACKIFG;

        /* Set the error code */
        err = -1;
    }

    return err;
}

/**
 * \brief Transmit data to the slave device
 * \param[in] dev - the I2C slave device
 * \param[in] buf - the buffer of data to transmit
 * \param[in] nbytes - the number of bytes to transmit
 * \return 0 on success, -1 otherwise
 */
static int _transmit(const struct i2c_device *dev, const uint8_t *buf, size_t nbytes)
{
    int err = 0;
    IGNORE(dev);

    /* Send the start condition */
    UCB0CTL1 |= UCTR | UCTXSTT;        

    /* Wait for the start condition to be sent and ready to transmit interrupt */
    while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));

    /* Check for ACK */
    err = _check_ack(dev);
    
    /* If no error and bytes left to send, transmit the data */
    while ((err == 0) && (nbytes > 0)) {
        UCB0TXBUF = *buf;
        while ((IFG2 & UCB0TXIFG) == 0) {
            err = _check_ack(dev);
            if (err < 0) {
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
 * \param[in] dev - the I2C slave device
 * \param[in] buf - the buffer to store the received data
 * \param[in] nbytes - the number of bytes to receive
 * \return 0 on success, -1 otherwise
 */
static int _receive(const struct i2c_device *dev, uint8_t *buf, size_t nbytes)
{
    int err = 0;
    IGNORE(dev);

    /* Send the start and wait */
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;

    /* Wait for the start condition to be sent */
    while (UCB0CTL1 & UCTXSTT);
    
    /* 
     * If there is only one byte to receive, then set the stop
     * bit as soon as start condition has been sent
     */
    if (nbytes == 1) {
        UCB0CTL1 |= UCTXSTP;
    }

    /* Check for ACK */
    err = _check_ack(dev);

    /* If no error and bytes left to receive, receive the data */
    while ((err == 0) && (nbytes > 0)) {
        /* Wait for the data */
        while ((IFG2 & UCB0RXIFG) == 0);

        *buf = UCB0RXBUF;
        buf++;
        nbytes--;

        /* 
         * If there is only one byte left to receive
         * send the stop condition
         */
        if (nbytes == 1) {
            UCB0CTL1 |= UCTXSTP; 
        }
    }

    return err;
}
