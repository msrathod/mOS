/** 
 * @file 	ismpframe.h
 * @author 	Mohit Rathod
 * Created: 08 10 2022, 03:44:44 pm
 * -----
 * Last Modified: 10 10 2022, 08:54:21 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   i2c Server Message Protocol(ISMP) definition.
 *  
 *  ISMP frame format
 * 
 *        |Header|Length|<--------Payload--------------->|Checksum|
 *        +------+------+----+------+------+      +------+--------+
 *        | Head | len  |Port|Data_1|Data_2|      |Data_n|   CRC  |
 *        +------+------+----+------+------+  ''' +------+--------+
 *           0      1      2    3      4            n+2     n+3
 *  Max Data payload = N bytes
 *  Max packet length = N + 3 bytes
 *  Head    => ISMP Header          (1 Byte)
 *  Len     => Length of payload    (1 Byte)
 *  Port    => i2c Server Port      (1 Byte) 
 *  Data_x  => Payload Data         (Len-1 Byte)
 *  CRC     => CRC8 of the frame    (1 Byte) 
 * 
 */
#ifndef utils_ismpframe_h
#define utils_ismpframe_h
#include <stdint.h>

#define MAX_PAYLOAD_LEN         (4)
#define MAX_PACKET_LEN          (MAX_PAYLOAD_LEN + 3)
#define ISMP_SVC_HEADER         (0x80)
#define ISMP_RSP_HEADER         (0x55)

typedef union
{
    struct
    {
        uint8_t Header;
        uint8_t Len;
        uint8_t Port;
        uint8_t Data[MAX_PAYLOAD_LEN - 1];
        uint8_t Checksum;
    };
    uint8_t buf[MAX_PACKET_LEN];
} ISMPframe_t;
 
typedef enum{
    FRAME_OK                = 0x00,
    FRAME_OK_SRVC_BUSY,
    HEADER_ERROR            = 0x51,
    CHECKSUM_ERROR,
    FRAME_SIZE_ERROR,
    INVALID_PARAMS,
    INVALID_SRVC,
    INVALID_PORT,
    ISMP_ONGOING,
    UNKNOWN_ERROR,
    UNKNOWN_RESP,
    ISMP_VERSION            = 0xC0
} ISMPresponse_t;

typedef enum port_enum{
    PORT_0 = 0xA0,   // provides service 0
    PORT_1,          // provides service 1
    PORT_2,          // provides service 2
    PORT_3,          // provides service 3
    PORT_4,          // provides service 4
    PORT_5,          // provides service 5
    PORT_6,          // provides service 6
    PORT_7,          // provides service 7
    PORT_MAX
} ISMPport_t;

#endif /* utils_ismpframe_h */