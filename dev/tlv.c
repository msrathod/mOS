/** 
 * @file 	tlv.c
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 07:41:41 pm
 * -----
 * Last Modified: 23 09 2022, 07:46:34 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Tag Length Value (TLV) integrity checking API
 * 
 */
#include <mcu.h>
#include <stdint.h>
#include <stddef.h>
#include "tlv.h"

static uint16_t _calculate_checksum(uint16_t *address, size_t len);

int tlv_verify()
{
    return (TLV_CHECKSUM + _calculate_checksum((uint16_t *) 0x10C2, 62));
}

static uint16_t _calculate_checksum(uint16_t *data, size_t len)
{
    uint16_t crc = 0;
    len = len / 2;
    while (len-- > 0) {
        crc ^= *(data++);
    }
    return crc;
}