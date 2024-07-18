/** 
 * @file 	smpserver.c
 * @author 	Mohit Rathod
 * Created: 18 07 2024, 08:19:07 am
 * -----
 * Last Modified: 18 07 2024, 02:42:01 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief 
 * 
 */
#include <string.h>
#include <stdbool.h>
#include <utils/smpserver.h>
#include <utils/slip.h>


static SLIPframe_t _SLPpkt;
static uint8_t idx = 0;
static pkt_status_t pkt_status = EMPTY;

/* Service format */
typedef struct
{
  srvcfn_t pService;
  void *param;
  uint8_t len;
  uint8_t rsp;
  volatile uint16_t run;
}services_t;

static services_t _srvc[SMP_PORT_MAX];


static SMPresponse_t response = UNKNOWN_ERROR;

static void _flush_packet(SLIPframe_t *pckt);
static void packet_processor(SLIPframe_t *packet);
static bool isValidPort(SMPport_t portNum);

static void _flush_packet(SLIPframe_t *pckt)
{
  idx = 0;
  pkt_status = EMPTY;
  memset(pckt->buf, 0, MAX_PACKET_LEN);
}


/**
 * @brief SFMP_Manager_init:
 *        Function to initiailize the SFMP packet manager.
 * @return 0 on SUCCESS or -1 on FAILURE.      
 */
int sfmp_init()
{
    _flush_packet(&_SLPpkt);
    return 0;
}

/**
 * @brief SLP_Manager: 
 *            Reads the packet from UART buffers and process 
 *            them accordingly. Must be run periodically.
 * @return None.
 */
void smp_manager()
{
  uint8_t recvd;
  recvd = idx;

  if( recvd < MAX_PACKET_LEN )
  {
    
    recvd += slip_read( &(_SLPpkt.buf[recvd]), MAX_PACKET_LEN - recvd,
                               &(pkt_status));
    idx = recvd;
  }

  if((pkt_status == COMPLETE)||(recvd == MAX_PACKET_LEN))
  {      
/*     if(packet_processor(&SLP) == SUCCESS)
    {
      slip_write(SLP.pkt->buf, SLP.idx);
    } */
    _flush_packet(&_SLPpkt);  
  }  
}



/**
 * @brief SMP Server initialization
 *        Initialize the service arrays.
 * @param None
 * @return 0 on success, -1 otherwise
 */
int smp_init()
{
    SMPport_t portID;
    for(portID = PORT_0; portID < (PORT_0 + SMP_PORT_MAX); portID++)
    {
        smp_delService(portID);
    }
    return 0;
}

/**
 * @brief SMP_addService function to add a service to the server.
 * 
 * @param[in] service_fn - the name of the function which is to be registered.
 *            @note All service functions must be of type @ref srvfn_t.
 * 
 * @param[in] len - length of the parameter buffer.
 * @param[in] pbuf - buffer to store parameters for this service.
 * @param[in] port - the SMP_port to which service_fn needs to be attached.
 *                   must be a valid SMP_port_t value
 * @return 0 on success, -1 otherwise.  
 */
int smp_addService(srvcfn_t pService, const uint16_t len, uint8_t *pbuf, SMPport_t portID)
 {
    int ret = -1;
    if(isValidPort(portID))
    {
        if((_srvc[portID-PORT_0].pService == NULL) && (pService != NULL))
        {
            _srvc[portID-PORT_0].pService = pService;
            _srvc[portID-PORT_0].param = pbuf;
            _srvc[portID-PORT_0].len = len;
            _srvc[portID-PORT_0].rsp = UNKNOWN_RESP;
            _srvc[portID-PORT_0].run = 0;
            ret = 0;
        }
    }
    return ret;
}

/**
 * @brief SMP_delService function to remove a service from the SMP server.
 * @note: That this does *not* delete the associated function from
 *        memory it simply means that it is no longer serviced by the
 *        SMP server.
 * @param[in] portID - port no. of the service to be removed.
 * 
 * @return 0 on success, -1 otherwise.  
 */
int smp_delService(SMPport_t portID)
{
    int ret = -1;
    if(isValidPort(portID))
    {
        _srvc[portID-PORT_0].pService = NULL;
        _srvc[portID-PORT_0].param = NULL;
        _srvc[portID-PORT_0].len = 0;
        _srvc[portID-PORT_0].rsp = 0;
        _srvc[portID-PORT_0].run = 0;
        ret = 0;
    }
    return ret;
}

/**
 * @brief Service job dispatcher.
 *        When a service (function) is due to run, this function will run it.
 *        This function must be called (repeatedly) from the main loop.
 * @param none.
 * @return none.
 */
void smp_run()
{
    SMPport_t portID;
    for(portID = PORT_0; portID < (PORT_0 + SMP_PORT_MAX); portID++)
    {
        if(_srvc[portID-PORT_0].pService)
        {
            if(_srvc[portID-PORT_0].run > 0) {
                _srvc[portID-PORT_0].rsp = _srvc[portID-PORT_0].pService(_srvc[portID-PORT_0].param);
                /* critical section ? */
                _srvc[portID-PORT_0].run--;
            }
        }
    }
}

static bool isValidPort(SMPport_t portNum)
{
	bool ret = false;
	if ((portNum >= PORT_0) && (portNum < (PORT_0 + SMP_PORT_MAX))) {
		ret = true;
	}
	return ret;
}

static void packet_processor(SLIPframe_t *packet)
{
	if (_srvc[packet->Port - PORT_0].run != 0) {
		response = FRAME_OK_SRVC_BUSY;
	}
	else {
		_srvc[packet->Port - PORT_0].run++;
		/* check for null buffers in param */
		if (_srvc[packet->Port - PORT_0].param) {
			memcpy(_srvc[packet->Port - PORT_0].param, packet->Data, _srvc[packet->Port - PORT_0].len);
		}
		response = FRAME_OK;
	}
}
