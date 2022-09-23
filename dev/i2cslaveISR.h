/** 
 * @file 	i2cslaveISR.h
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 11:10:46 pm
 * -----
 * Last Modified: 23 09 2022, 11:14:33 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   ISR for I2C slave driver. This ISR also handles the
 *          UART interrupts to avoid conflict with Serial Driver
 *          when both modules are used simultaneously.
 * 
 */

#if MOS_USES(UART) == 1
#include <utils/queue.h>
extern qid_t serial_qID;
#endif

/* Tx and Rx interrupt service routine */
__attribute__((interrupt(USCIAB0TX_VECTOR))) void USCIAB0TX_ISR(void)
{
    /* I2C Tx ISR */
    if(IFG2 & UCB0TXIFG)
    {
    _transmit_callback(&UCB0TXBUF);
    }
    /* I2C Rx ISR */
    else if(IFG2 & UCB0RXIFG)
    {
    _receive_callback(UCB0RXBUF);
    }
}

/* Conflict with UART ISR.......... */
/* Start and Stop interrupt service routine */
__attribute__((interrupt(USCIAB0RX_VECTOR))) void USCIAB0RX_ISR(void)
{
    /* UART ISR */
    #if MOS_USES(UART) == 1
    if (IFG2 & UCA0RXIFG)
    {
        const char c = UCA0RXBUF;
        /* Clear the interrupt flag */
        IFG2 &= ~UCA0RXIFG;
        qEnqueue(serial_qID, &c);
    }
    #endif
    /* I2C state ISR */
    if (UCB0STAT & UCSTPIFG)            //Stop Interrupt
	{
        _state_callback();
		UCB0STAT &=	~UCSTPIFG;          //Clear STOP Flag
	}
	else if (UCB0STAT & UCSTTIFG)       // Start Interrupt
	{
        _state_callback();  
		UCB0STAT &= ~UCSTTIFG;          //Clear START Flag
	}
}

