/** 
 * @file 	icsserver.c
 * @author 	Mohit Rathod
 * Created: 10 10 2022, 09:56:18 pm
 * -----
 * Last Modified: 10 10 2022, 10:55:33 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C Simple Slave Server implementation
 * 
 */
#include "icsserver.h"

/* Service format */
typedef struct
{
  srvfn_t pService;
  void *param;
  uint8_t len;
  uint8_t rsp;
  volatile uint16_t run;
}services_t;

static services_t i2csrvc[PORT_NUM_MAX];

static ISMPframe_t packet;
static ISMPresponse_t response = UNKNOWN_RESP;

static int rsp_port = PORT_0;

static void stateCallback(void);
static void txCallback(volatile uint8_t *txdata);
static void rxCallback(const uint8_t recvd);
static void pushpacket(void);

int ICSserver_init()
{
    ISMPport_t portID;
    for (portID = PORT_0; portID < PORT_0 + PORT_NUM_MAX; portID++) {
        ICS_delService(portID);
    }
    /* Enable the i2c dev in slave mode. */
    return 0;
}