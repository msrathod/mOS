/** 
 * @file 	slip.h
 * @author 	Mohit Rathod
 * Created: 17 07 2024, 06:43:21 pm
 * -----
 * Last Modified: 17 07 2024, 07:40:25 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Serial Line IP implementation per RFC1055
 * 
 * @see https://tools.ietf.org/html/rfc1055
 *      the following C language functions send and 
 *      receive SLIP Packets.
 * 
 */
#ifndef utils_slip_h
#define utils_slip_h
#include<stdint.h>

typedef enum
{
  PENDING     = (uint8_t)0xFF,
  COMPLETE    = (uint8_t)0x0F,
  EMPTY       = (uint8_t)0xF0,
  STATUS_ERR
}pkt_status_t;

/**
 * @brief SLIP packet structure
 */
typedef struct {
  uchar_t* buf;               /* SLIP packet buffer */
  uint8_t idx;                /* Buffer index(length) for incomplete(complete) package */
  //int8_t len;                 // packet length (0 to 127)
  pkt_status_t pkt_status;    /* Packet status */
}SLIP_PACKET_t;

/**
 * @brief SLIP special character code 
 */
#define SLIP_END             ((uint8_t)0300)    /* indicates end of packet */
#define SLIP_ESC             ((uint8_t)0333)    /* indicates byte stuffing */
#define SLIP_ESC_END         ((uint8_t)0334)    /* ESC_END -> END byte */
#define SLIP_ESC_ESC         ((uint8_t)0335)    /* ESC_ESC -> ESC byte */

/**
 * @fn      int8_t slip_recv_packet(uchar_t *, int8_t, pkt_status_t *);
 * @brief   RECV_PACKET: receives a packet into the buffer located at "p".
 * @param   p       pointer to the receive buffer
 * @param   len     number of bytes to be received
 * @param   status  pointer to packet status 
 * @return  Returns the number of bytes stored in the buffer.
 * 
 * @note:   
 *          If more than len bytes are received, the packet will be truncated.
 */
int8_t slip_recv_packet(uchar_t *p, int8_t len, pkt_status_t *status);

/**
 * @fn      void slip_send_packet(uchar_t *p, int8_t len);
 * @param   p       pointer to the receive buffer
 * @param   len     length of packet
 * @return  void 
 * @brief   SEND_PACKET: sends a packet of length "len", starting at
 *          location "p".
 */
void slip_send_packet(uchar_t *p, int8_t len);

#endif /* utils_slip_h */