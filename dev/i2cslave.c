/** 
 * @file 	i2cslave.c
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 11:00:46 pm
 * -----
 * Last Modified: 23 09 2022, 11:15:00 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C Slave driver APIs
 * 
 */
#include <mosconfig.h>
#include <mcu.h>
#include "i2cslave.h"

void (*_receive_callback)(const uint8_t receive);
void (*_transmit_callback)(volatile uint8_t *send_next);
void (*_state_callback)(void);

void i2cslave_init(void (*State_Callback)(),
                   void (*Tx_Callback)(volatile uint8_t *value),
				   void (*Rx_Callback)(const uint8_t value),
				   uint8_t slave_address)
{
    /* Register callback functions */
    _state_callback = State_Callback;
    _receive_callback = Rx_Callback;
    _transmit_callback = Tx_Callback;
    /* Configure the USCI_B module */
    UCB0CTL1 |= UCSWRST;                // Enable SW reset
	UCB0CTL0 = UCMODE_3 + UCSYNC;       // I2C Slave, synchronous mode
	UCB0I2COA = slave_address;          // set own (slave) address
	UCB0CTL1 &= ~UCSWRST;               // Clear SW rst, resume operation
	IE2 |= UCB0TXIE + UCB0RXIE;         // Enable RX/TX interrupt
	UCB0I2CIE |= UCSTPIE + UCSTTIE;     // Enable STT/STP interrupt
}

#if MOS_USES(I2C) == 2
#include <dev/i2cslaveISR.h>
#endif
