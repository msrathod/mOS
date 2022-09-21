/** 
 * @file 	serialISR.h
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 10:20:09 pm
 * -----
 * Last Modified: 21 09 2022, 10:27:08 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Interrupt service routine for serial module.
 *          This file is included in the driver when the
 *          serial rx mode is also implemented and the sys
 *          -tem doesn't use I2C module interrupts.
 */
__attribute__((interrupt(USCIAB0RX_VECTOR))) void USCIAB0RX_ISR(void)
{
    /* UART ISR */
    if (IFG2 & UCA0RXIFG) {
        const char c = UCA0RXBUF;
        /* Clear the interrupt flag */
        IFG2 &= ~UCA0RXIFG;
        qEnqueue(serial_qID, &c);
    }
}