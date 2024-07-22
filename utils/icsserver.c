/** 
 * @file 	icsserver.c
 * @author 	Mohit Rathod
 * Created: 10 10 2022, 09:56:18 pm
 * -----
 * Last Modified: 22 07 2024, 06:02:18 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C Simple Slave Server implementation
 * 
 */
#include <icsserver.h>
#include <dev/i2c.h>
#include <utils/crc8.h>
#include <utils/server.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

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

static Serverstate_t state;
static ISMPframe_t packet;
static ISMPresponse_t response = UNKNOWN_RESP;

static int rsp_port = PORT_0;

static serverID_t icsID;

static void stateCallback(void);
static void txCallback(volatile uint8_t *txdata);
static void rxCallback(const uint8_t recvd);
static void pushpacket(void);
static bool isValidPort(ISMPport_t portNum);

int ICSserver_init(int numService)
{
	int ret;
	ret = server_init(&icsID, numService);
    /* Enable the i2c dev in slave mode. */
    i2cslave_init(stateCallback, txCallback, rxCallback, ICS_SERVER_ADDRESS);
    return ret;
}

int ICS_addService(srvfn_t pService, const uint16_t len, void *pbuf, portID_t portID)
{
	return addService(icsID, pService, len, pbuf, portID);
}

int ICS_delService(portID_t portID)
{
	return delService(icsID, portID);
}

void ICS_run()
{
	server_run(icsID);
}

static void stateCallback(void)
{
	switch (state)
	{
	case(BAD_FRAME):
		/* reset state */
		state = HEADER_MODE;
		break;
	
	default:
		break;
	}

}

static void txCallback(volatile uint8_t *txdata)
{
	switch(state)
	{
		case(SRVC_RSP_MODE):
			state = HEADER_MODE;
			*txdata = getServiceRsp(icsID, rsp_port);
			break;
		case(PKT_RSP_MODE):
			state = HEADER_MODE;
			*txdata = response;
			response = UNKNOWN_RESP;
			break;
		default:
			/* no operation */
			break;
	}
}

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
			if ((rxdata > 0) && (rxdata <= MAX_PAYLOAD_LEN)) {
				state = PORT_MODE;
				response = ISMP_ONGOING;
				packet.Len = rxdata;
				idx = rxdata;
			}
			else {
				state = BAD_FRAME;
				response = FRAME_SIZE_ERROR;
			}
			break;
		case(PORT_MODE):
			if (isValidPort(rxdata)) {
				state = DATA_MODE;
				packet.Port = rxdata;
				/* if no service registered at this portID */
				if (!(isValidService(icsID, packet.Port)))
				{
					state = BAD_FRAME;
					response = INVALID_SRVC;
				}
				/* if not enough parameters for the service at this portID */
				else if (getParamLen(icsID, packet.Port) != (packet.Len - 1))
				{
					state = BAD_FRAME;
					response = INVALID_PARAMS;
				}
			}
			else {
				state = BAD_FRAME;
				response = INVALID_PORT;
			}
			break;
		case(DATA_MODE):
			if (idx > 1) {
				packet.Data[packet.Len - idx] = rxdata;
				idx--;
				break;
			}
			state = CRC_MODE;
		case(CRC_MODE):
			/* Last byte in the packet is the checksum */
			packet.buf[packet.Len + 2] = rxdata;
			if (computeFCS(packet.buf, (packet.Len+3))) {
				/* Bad packet let the host know and discard the frame. */
				state = BAD_FRAME;
				response = CHECKSUM_ERROR;
			}
			else {
				state = HEADER_MODE;
				response = FRAME_OK;
				pushpacket();
			}
			break;
		case(BAD_FRAME):
		default:
			/* no_operation */
		break;
	}
}
static void pushpacket(void)
{
	int ret = pushParam2Service(icsID, packet.Port, packet.Data);
	if (ret == -3)
	{
		response = FRAME_OK_SRVC_BUSY;
	}
	else if(ret == 0) 
	{
		response = FRAME_OK;
	}
}

static bool isValidPort(ISMPport_t portNum)
{
	bool ret = false;
	if ((portNum >= PORT_0) && (portNum < (PORT_0 + PORT_NUM_MAX))) {
		ret = true;
	}
	return ret;
}
