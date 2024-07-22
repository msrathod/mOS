/** 
 * @file 	server.c
 * @author 	Mohit Rathod
 * Created: 20 07 2024, 07:02:30 am
 * -----
 * Last Modified: 22 07 2024, 05:58:38 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief services utilities api implementation.
 * 
 */
#include <mos.h>
#include <utils/server.h>
#include <stdlib.h>
#include <string.h>

#if MOS_GET(MAX_SERVERS)
#define SERVER_MAX              MOS_GET(MAX_SERVERS)
#else
#define SERVER_MAX              (2)
#endif

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
    services_t *pServices;      /* services offered by server */
    size_t portMax;             /* num of services (also port occupied) */
} server_t;

static server_t _Server[SERVER_MAX];
static size_t serverIDx = 0;

static bool _isValidPort(portID_t portID, size_t portMax);
static bool _isValidServer(serverID_t serverID, size_t serverMax);
static void _delService(services_t *ptr, portID_t portID);
static void _addService(services_t*ptr, srvfn_t pService, const uint16_t len, void *pbuf, portID_t portID)
static inline void _pushParam(services_t *ptr, portID_t portID, void *pdata);

/**
 * @brief Services initialization
 *        Initialize the services array for the server and generates its
 *        unique ID.
 * @param   pID pointer to serverID for the initialized server.
 * @param   numPort number of ports used for server    
 * @return  0 on success, 
 *         -1 max server init calls or null pointers
 *         -2 for invalid services pointer in server 
 */
int server_init(serverID_t *pID, int numPort)
{
    static serverID_t _idx = 0;
    int ret = -1;
    portID_t portID;
    if((_idx < SERVER_MAX) && (pID != NULL) && (numPort > 0))
    {
        _Server[_idx].portMax = numPort;
        _Server[_idx].pservices = (services_t*) malloc(numPort * sizeof(services_t)); 
        //@todo add a check for null ptr from malloc
        for(portID = PORT_0; portID < (PORT_0 + numPort); portID++) 
        {
            _delService(_Server[_idx].pServices, portID);
        }
        *pID = _idx++;
        serverIDx = _idx;
        ret = 0;
    }
    return ret;
}

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
int addService(serverID_t serverID, srvfn_t pService, const uint16_t len, void *pbuf, portID_t portID)
{
	int ret = -1;
    if(_isValidServer(serverID, serverIDx))
    {
        ret = -2;
        if(_isValidPort(portID, _Server[serverID].portMax))
        {
            ret = -3;
            if ((_Server[serverID].pServices[portID - PORT_0].pService == NULL) && (pService != NULL)) 
            {
                _addService(_Server[serverID].pServices, pService, len, pbuf, portID);
               ret = 0;
            }
        }
    }
	return ret;
}

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
int delService(serverID_t serverID, portID_t portID)
{
	int ret = -1;
    if(_isValidServer(serverID, serverIDx))
    {
        ret = -2;
        if(_isValidPort(portID, _Server[serverID].portMax)) 
        {
            _delService(_Server[serverID].pServices, portID);
            ret = 0;
        }
    }
	return ret;
}

/**
 * @brief   Queries the service status or validity at a given port(portID) on
 *          the provided server (serverID).
 * @param   serverID - server identifier  
 * @param   portID - port identifier
 * @return  true - if service exists 
 * @return  false - otherwise
 */
bool isValidService(serverID_t serverID, portID_t portID)
{
    bool ret = false;
    if(_isValidServer(serverID, serverIDx))
    {
        if(_isValidPort(portID, _Server[serverID].portMax)) 
        {
            if(_Server[serverID].pServices[portID-PORT_0].pService){
                ret = true;
            }
        }
    }
	return ret;
}

/**
 * @brief   Push parameters to a service function at port portID on the 
 *          server serverID.
 * @param   serverID - server identifier
 * @param   portID - port identifier
 * @return  0 on sucess, -1 otherwise
 */
int pushParam2Service(serverID_t serverID, portID_t portID, void *data)
{
    int ret = -1;
    if(_isValidServer(serverID, serverIDx))
    {
        ret = -2;
        if(_isValidPort(portID, _Server[serverID].portMax)) 
        {
            ret = -3;
            /* check for busy service pending runs, if free then add params */
            if(_Server[serverID].pServices[portID-PORT_0].run == 0)
            {
                _Server[serverID].pServices[portID-PORT_0].run++;
                _pushParam(_Server[serverID].pServices, portID);
                ret = 0;
            }
        }
    }
	return ret;
}

/**
 * @brief Get the param len for the service at portID on server serverID. 
 * @param   serverID - server identifier
 * @param   portID   - port identifier
 * @return  len of param on success
 *          -1 on invalid serverID
 *          -2 on invalid portID
 */
int getParamLen(serverID_t serverID, portID_t portID)
{
    int ret = -1;
    if(_isValidServer(serverID, serverIDx))
    {
        ret = -2;
        if(_isValidPort(portID, _Server[serverID].portMax)) 
        {
            ret = _Server[serverID].pServices[portID - PORT_0].len;
        }
    }
	return ret;
}

/**
 * @brief Get the Service Rsp for the service specified at portID on
 *        server serverID.
 * @param   serverID 
 * @param   portID 
 * @return  respone of service action 
 */
int getServiceRsp(serverID_t serverID, portID_t portID)
{
    int ret = -1;
    if(_isValidServer(serverID, serverIDx))
    {
        ret = -2;
        if(_isValidPort(portID, _Server[serverID].portMax)) 
        {
            ret = _Server[serverID].pServices[portID - PORT_0].rsp;
        }
    }
	return ret;
}

/**
 * @brief   Server services dispatcher.
 *          When a service (function) is due to run, this function will run it.
 *          This function must be called (repeatedly) from the main loop.
 * @param   serverID -  server id for which the dispatcher is requested. 
 * @return  none 
 */
void server_run(serverID_t serverID)
{
    if(_isValidServer(serverID, serverIDx))
    {
        portID_t portID;
        for (portID = PORT_0; portID < (PORT_0 + _Server[serverID].portMax); portID++) 
        {
            if(_Server[serverID].pServices[portID-PORT_0].pService) 
            {
                if(_Server[serverID].pServices[portID-PORT_0].run > 0){
                    _Server[serverID].pServices[portID-PORT_0].rsp = _Server[serverID].pServices[portID-PORT_0].pService(_Server[serverID].pServices[portID-PORT_0].param);
                    _Server[serverID].pServices[portID-PORT_0].run--;
                }
            }

        }
    }
}

static void _delService(services_t *ptr, portID_t portID)
{
    ptr[portID - PORT_0].pService = NULL;
    ptr[portID - PORT_0].param = NULL;
    ptr[portID - PORT_0].len = 0;
    ptr[portID - PORT_0].rsp = 0;
    ptr[portID - PORT_0].run = 0;
}

static bool _isValidPort(portID_t portNum, size_t portMax)
{
	bool ret = false;
	if ((portNum >= PORT_0) && (portNum < (PORT_0 + portMax))) {
		ret = true;
	}
	return ret;
}

static bool _isValidServer(serverID_t serverID, size_t serverMax)
{
	bool ret = false;
	if ((serverID >= 0) && (serverID < serverMax)) {
		ret = true;
	}
	return ret;
}

static void _addService(services_t*ptr, srvfn_t pService, const uint16_t len, void *pbuf, portID_t portID)
{
    ptr[portID - PORT_0].pService = pService;
    ptr[portID - PORT_0].param = pbuf;
    ptr[portID - PORT_0].len = len;
    ptr[portID - PORT_0].rsp = 0;
    ptr[portID - PORT_0].run = 0;
}

static inline void _pushParam(services_t *ptr, portID_t portID, void *pdata)
{
    uint16_t len = ptr[portID - PORT_0].len;
    memcpy(ptr[portID - PORT_0].param, pdata, len);
}