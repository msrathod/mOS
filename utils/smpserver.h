/** 
 * @file 	smpserver.h
 * @author 	Mohit Rathod
 * Created: 18 07 2024, 07:46:41 am
 * -----
 * Last Modified: 18 07 2024, 08:51:20 am
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Slip framed message protocol server implementation.
 * 
 */
#ifndef utils_smpserver_h
#define utils_smpserver_h

#include <stdint.h>
#include <utils/slipframe.h>
/* Maximum service port supported by the SMP Server. */
#define SMP_PORT_MAX            (4)

/** SERVICES are later mapped to actual functions when app calls this
 *  utility and assigns a service to said port. This will become the 
 *  port to which a SMP client will write to get the desired action.*/

/* Service (function) type */
typedef uint8_t (*srvcfn_t)(void *);


/**
 * @brief SMP Server initialization
 *        Initialize the service arrays.
 * @param None
 * @return 0 on success, -1 otherwise
 */
int smp_init();

/**
 * @brief SMP_addService function to add a service to the server.
 * 
 * @param[in] service_fn - the name of the function which is to be registered.
 *            @note All service functions must be of type @ref srvcfn_t.
 * 
 * @param[in] len - length of the parameter buffer.
 * @param[in] pbuf - buffer to store parameters for this service.
 * @param[in] port - the SMP_port to which service_fn needs to be attached.
 *                   must be a valid SMP_port_t value
 * @return 0 on success, -1 otherwise.  
 */
int smp_addService(srvcfn_t pService, const uint16_t len, uint8_t *pbuf, SMPport_t portID);

/**
 * @brief SMP_delService function to remove a service from the SMP server.
 * @note: That this does *not* delete the associated function from
 *        memory it simply means that it is no longer serviced by the
 *        SMP server.
 * @param[in] portID - port no. of the service to be removed.
 * 
 * @return 0 on success, -1 otherwise.  
 */
int smp_delService(SMPport_t portID);

/**
 * @brief Scheduler task dispatcher.
 *        When a task (function) is due to run, this function will run it.
 *        This function must be called (repeatedly) from the main loop.
 * @param none.
 * @return none.
 */
void smp_run();

#endif /* utils_smpserver_h */