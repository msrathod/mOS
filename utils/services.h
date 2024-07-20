/** 
 * @file 	services.h
 * @author 	Mohit Rathod
 * Created: 19 07 2024, 07:09:39 am
 * -----
 * Last Modified: 19 07 2024, 10:31:00 am
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
 * @brief Service function template. */
typedef uint8_t (*srvfn_t)(void *);

/* Service format */
typedef struct
{
	srvfn_t pService;       /* service function */
	void *param;            /* params buffer */
	uint8_t len;            /* length of param */
	uint8_t rsp;            /* return response of service */
	volatile uint16_t run;  /* run flag for service execution */
} services_t;

typedef struct
{
    services_t services[];
    size_t num;
} server_t;

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
} SVCport_t;

/**
 * @brief Services initialization
 *        Initialize the service arrays.
 * @param pServices pointer to services buffer for server
 * @param num       number of ports used for server    
 * @return void
 */
void services_init(server_t *pServer);
void services_init(server_t *pServer)
{
    SVCport_t portID;
    for (portID = PORT_0; portID < (PORT_0 + pServer->num); portID++) {
        _delService(pServer->services, portID);
    }
}

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
int ICS_delService(ISMPport_t portID)
{
	int ret = -1;
    if(isValidPort(portID)) {
		srvc[portID - PORT_0].pService = NULL;
		srvc[portID - PORT_0].param = NULL;
		srvc[portID - PORT_0].len = 0;
		srvc[portID - PORT_0].rsp = 0;
		srvc[portID - PORT_0].run = 0;
		ret = 0;
	}
	return ret;
}

/**
 * @brief   ICS Server service dispatcher.
 *          When a service (function) is due to run, this function will run it.
 *          This function must be called (repeatedly) from the main loop.
 * @param   none.
 * @return none.
 */
void ICS_run(void);

#endif /* utils_services_h */