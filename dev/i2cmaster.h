/** 
 * @file 	i2cmaster.h
 * @author 	Mohit Rathod
 * Created: 21 07 2024, 05:10:58 pm
 * -----
 * Last Modified: 21 07 2024, 05:28:52 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C master driver. Configures the USCI_B module as
 *          master capable of transmitting and receiving bytes.
 * -------------------------------------------------------------------  
 *                 Slave
 *              MSP430G2553
 *          -----------------
 *      /|\|              XIN|-
 *       | |                 |
 *       --|RST          XOUT|-
 *         |                 |
 *         |                 |
 *         |                 |
 *         |         SDA/P1.7|------->
 *         |         SCL/P1.6|------->
 * 
 * Note: External pull-ups are needed for SDA & SCL
 * -------------------------------------------------------------------
 * 
 */
#ifndef dev_i2c_master_h
#define dev_i2c_master_h
#include <stdint.h>
#include <stddef.h>

/* I2C slave device structure */
struct i2c_device
{
    uint8_t address;
};

/* I2C transaction data */
struct i2c_data
{
    const void *tx_buf;
    size_t tx_len;
    void *rx_buf;
    size_t rx_len;
};

/**
 * \brief Initialize the I2C peripheral
 * \return 0 on success, -1 otherwise
 */
int i2c_init(void);

/**
 * \brief Perform an I2C transfer
 * \param[in] dev - the I2C slave device
 * \param[in/out] data - data structure containing the buffers
 * \return 0 on success, -1 otherwise
 */
int i2c_transfer(const struct i2c_device *dev, struct i2c_data *data);


#endif /* dev_i2c_master_h */