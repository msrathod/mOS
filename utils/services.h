/** 
 * @file 	services.h
 * @author 	Mohit Rathod
 * Created: 19 07 2024, 07:09:39 am
 * -----
 * Last Modified: 20 07 2024, 11:15:44 am
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   provides the services utilities for various
 *          server implementation.
 * 
 */
#ifndef utils_services_h
#define utils_services_h
#include <stdint.h>
#include <stdbool.h>

/**
 * @typedef uint8_t (*srvfn_t)(void *);
 * @brief   Service function template. 
 */
typedef uint8_t (*srvfn_t)(void *);

/**
 * @typedef typedef unsigned int serverID_t;
 * @brief   Server identifier
 *          Desribes the server for whose services needs to be accessed.
 *          A unique value represents a unique server.
 */
typedef unsigned int serverID_t;

typedef enum port_enum{
    PORT_0 = 0xA0,          // provides service 0
    PORT_1,                 // provides service 1
    PORT_2,                 // provides service 2
    PORT_3,                 // provides service 3
    PORT_4,                 // provides service 4
    PORT_5,                 // provides service 5
    PORT_6,                 // provides service 6
    PORT_7,                 // provides service 7
    PORT_MAX
} portID_t;

/**
 * @brief Services initialization
 *        Initialize the services array for the server and generates its
 *        unique ID.
 * @param   pID pointer to serverID for the initialized server.
 * @param   num number of ports used for server    
 * @return   0 on success, 
 *          -1 max server init calls or null pointers
 *          -2 for invalid services pointer in server 
 */
int services_init(serverID_t *pID, int num);

/**
 * @brief    addServices function to add a service to the server.
 * 
 * @param serverID      the server to which the service needs to be added.
 *                      @ref serverID_t
 * @param service_fn    the name of the function which is to be registered.
 *                      @note All service functions must be of type 
 *                      @ref srvfn_t
 * @param len           length of the parameter buffer.
 * @param pbuf          buffer to store parameters for this service.
 * @param port          the port to which service_fn needs to be attached.
 *                      must be a valid @ref portID_t value
 * @return       0 on success,
 *              -1 if server ID is invalid
 *              -2 if portID is invalid
 *              -3 if service is invalid or already registered at this port
 */
int addService(serverID_t serverID, srvfn_t pService, const uint16_t len, void *pbuf, portID_t portID);

/**
 * @brief   delService function to remove a service from the server.
 * @note:   This does *not* delete the associated function from memory 
 *          it simply means that it is no longer serviced by the server.
 * @param   serverID server identifier
 * @param   portID port no. of the service to be removed.
 * 
 * @return   0 on success, 
 *          -1 if serverID is invalid, 
 *          -2 if portID is invalid.  
 */
int delService(serverID_t serverID, portID_t portID);

bool isValidService(serverID_t serverID, portID_t portID);
int get
/**
 * @brief   ICS Server service dispatcher.
 *          When a service (function) is due to run, this function will run it.
 *          This function must be called (repeatedly) from the main loop.
 * @param   none.
 * @return none.
 */
void ICS_run(void);

#endif /* utils_services_h */