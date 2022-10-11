/** 
 * @file 	icsserver.h
 * @author 	Mohit Rathod
 * Created: 10 10 2022, 09:26:16 pm
 * -----
 * Last Modified: 11 10 2022, 02:18:45 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C Simple Slave Server implementation
 * 
 */
#ifndef utils_icsserver_h
#define utils_icsserver_h
#include <stdint.h>
#include <utils/ismpframe.h>

/* A maximum of 8 service ports are supported in this implementation. */
#define PORT_NUM_MAX            (4)

/* Server i2c address. */
#define ICS_SERVER_ADDRESS      (0x40)

/* Service (function) prototype */
typedef uint8_t (*srvfn_t)(void *);

/* SERVICES are later mapped to actual functions when applications calls
 * this utility and assigns a service to said register. This will become
 * the port to which an i2c master will write to get the desired action
 * out of the i2c slave device. */

/**
 * @brief I2C Slave Server initialization
 *        Initialize the service arrays.
 * @param None
 * @return 0 on success, -1 otherwise
 */
int ICSserver_init(void);

/**
 * @brief    ICS_addService function to add a service to the server.
 * 
 * @param service_fn    the name of the function which is to be registered.
 *                      @note All service functions must be of type 
 *                      @ref srvfn_t.
 * @param len           length of the parameter buffer.
 * @param pbuf          buffer to store parameters for this service.
 * @param port          the ISMPport to which service_fn needs to be attached.
 *                      must be a valid ISMPport_t value
 * @return 0 on success, -1 otherwise.  
 */
int ICS_addService(srvfn_t pService, const uint16_t len, void *pbuf, ISMPport_t portID);

/**
 * @brief   ICS_delService function to remove a service from the server.
 * @note:   This does *not* delete the associated function from memory 
 *          it simply means that it is no longer serviced by the server.
 * @param   portID port no. of the service to be removed.
 * 
 * @return 0 on success, -1 otherwise.  
 */
int ICS_delService(ISMPport_t portID);

/**
 * @brief   ICS Server service dispatcher.
 *          When a service (function) is due to run, this function will run it.
 *          This function must be called (repeatedly) from the main loop.
 * @param   none.
 * @return none.
 */
void ICS_run(void);

#endif /* utils_icsserver_h */