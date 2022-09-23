/** 
 * @file 	i2cslave.h
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 10:52:32 pm
 * -----
 * Last Modified: 23 09 2022, 10:59:43 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C slave driver. Configures the USCI_B module as
 *          slave capable of transmitting and receiving bytes.
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
#ifndef dev_i2c_slave_h
#define dev_i2c_slave_h
#include <stdint.h>

/**
 * @brief   Initialize the USCI module for I2C slave operation.
 * 
 * @param   State_Callback  Callback fn for START/STOP conditions
 * @param   Tx_Callback     Callback fn for byte requested by Master
 * @param   Rx_Callback     Callback fn for byte received from Master
 * @param   slave_address   Own address for slave mode
 * @return  void
 */
void i2cslave_init(void (*State_Callback)(),
                   void (*Tx_Callback)(volatile uint8_t *value),
				   void (*Rx_Callback)(const uint8_t value),
				   uint8_t slave_address);

#endif /* dev_i2c_slave_h */