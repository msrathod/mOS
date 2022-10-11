/** 
 * @file 	icsserver.c
 * @author 	Mohit Rathod
 * Created: 10 10 2022, 09:56:18 pm
 * -----
 * Last Modified: 11 10 2022, 10:35:55 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C Simple Slave Server implementation
 * 
 */
#include "icsserver.h"
#include <dev/i2c.h>
#include <utils/crc8.h>
#include <stddef.h>
#include <stdbool.h>

/* Service format */
typedef struct
{
	srvfn_t pService;
	void *param;
	uint8_t len;
	uint8_t rsp;
	volatile uint16_t run;
} services_t;

typedef enum serverState
{
	BAD_FRAME = 0x0F,
	HEADER_MODE,
	LENGTH_MODE,
	PORT_MODE,
	DATA_MODE,
	CRC_MODE,
	PKT_RSP_MODE,
	SRVC_RSP_MODE
} Serverstate_t;

static services_t i2csrvc[PORT_NUM_MAX];

static Serverstate_t state;
static ISMPframe_t packet;
static ISMPresponse_t response = UNKNOWN_RESP;

static int rsp_port = PORT_0;

static void stateCallback(void);
static void txCallback(volatile uint8_t *txdata);
static void rxCallback(const uint8_t recvd);
static void pushpacket(void);
static bool isValidPort(ISMPport_t portNum);

int ICSserver_init()
{
    ISMPport_t portID;
    for (portID = PORT_0; portID < (PORT_0 + PORT_NUM_MAX); portID++) {
        ICS_delService(portID);
    }
    /* Enable the i2c dev in slave mode. */
    i2cslave_init(stateCallback, txCallback, rxCallback, ICS_SERVER_ADDRESS);
    return 0;
}

int ICS_addService(srvfn_t pService, const uint16_t len, void *pbuf, ISMPport_t portID)
{
	int ret = -1;
	if ((portID >= PORT_0) && (portID < (PORT_0 + PORT_NUM_MAX))) {
		if ((i2csrvc[portID - PORT_0].pService == NULL) && (pService != NULL)) {
			i2csrvc[portID - PORT_0].pService = pService;
			i2csrvc[portID - PORT_0].param = pbuf;
			i2csrvc[portID - PORT_0].len = len;
			i2csrvc[portID - PORT_0].rsp = UNKNOWN_RESP;
			i2csrvc[portID - PORT_0].run = 0;
			ret = 0;
		}
	}
	return ret;
}

int ICS_delService(ISMPport_t portID)
{
	int ret = -1;
	if ((portID >= PORT_0) && (portID < (PORT_0 + PORT_NUM_MAX))) {
		i2csrvc[portID - PORT_0].pService = NULL;
		i2csrvc[portID - PORT_0].param = NULL;
		i2csrvc[portID - PORT_0].len = 0;
		i2csrvc[portID - PORT_0].rsp = 0;
		i2csrvc[portID - PORT_0].run = 0;
		ret = 0;
	}
}

void ICS_run()
{
	ISMPport_t portID;
	for (portID = PORT_0; portID < (PORT_0 + PORT_NUM_MAX); portID++) {
		if (i2csrvc[portID - PORT_0].pService) {
			if (i2csrvc[portID - PORT_0].run > 0) {
				i2csrvc[portID - PORT_0].rsp = i2csrvc[portID - PORT_0].pService(i2csrvc[portID - PORT_0].param);
				/* Critical Section? */
				i2csrvc[portID - PORT_0].run--;
			}
		}
	}
}

static void stateCallback(void)
{

}
static void txCallback(volatile uint8_t *txdata);
static void rxCallback(const uint8_t recvd)
{
	static int idx = 0;
	uint8_t rxdata = recvd;
	switch(state)
	{
		case(HEADER_MODE):
			/* If a portID is sent to enquire its service response. */
			if (isValidPort(rxdata)) {
				state = SRVC_RSP_MODE;
				rsp_port = rxdata;
			}
			/* or if current server state is requested */
			else if (rxdata == ISMP_RSP_HEADER) {
				state = PKT_RSP_MODE;
			}
			/* if a ISMP service header is sent to indicate an ISMP frame */
			else if (rxdata == ISMP_SVC_HEADER) {
				state = LENGTH_MODE;
				packet.Header = ISMP_SVC_HEADER;
				response = ISMP_ONGOING;
			}
			else {
				state = BAD_FRAME;
				response = HEADER_ERROR;
			}
		break;
		case(LENGTH_MODE):

		break;
	}
}
static void pushpacket(void);
static bool isValidPort(ISMPport_t portNum)
{
	bool ret = false;
	if ((portNum >= PORT_0) && (portNum < (PORT_0 + PORT_NUM_MAX))) {
		ret = true;
	}
	return ret;
}