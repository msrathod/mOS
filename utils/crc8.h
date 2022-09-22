/** 
 * @file 	crc8.h
 * @author 	Mohit Rathod
 * Created: 22 09 2022, 09:08:15 pm
 * -----
 * Last Modified: 22 09 2022, 09:33:20 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief CRC-8 implementation for frame checking. CDMA2000
 * @note CRC-8 implementation uses 0xFF as initial value and 0x9B as polynomial
 * @see https://crccalc.com/?crc=0x31%200x32%200x33%200x34%200x35%200x36%200x37%200x38%200x39&method=crc8&datatype=hex&outtype=hex
 * 
 */
#ifndef utils_crc8_h
#define utils_crc8_h
#include <stdint.h>

/**
 * @fn      int validate_crc8();
 * @brief   Test the validity of the CRC8 implementation.
 *          Validates the implementation by computing the crc8 over
 *          a standard sequence {0x31, 0x32, ..., 0x39} & comparing
 *          it with the value of the CRC8/CDMA2000 standard.
 * @param   void
 * @return  0 on success, -1 otherwise
 */
int validateCRC8(void);

/**
 * @fn      int computeFCS(const uint8_t *pframe, int len);
 * @brief   Compute the frame check sequence for a given frame.
 * @param   pframe  pointer to the frame buffer
 * @param   len     frame length
 * @return  fcs value on success, -1 otherwise
 */
int computeFCS(const uint8_t *pframe, int len);

/**
 * @fn      int computeCRC(const uint8_t crc0, const uint8_t *pbuf, int len);
 * @brief   compute the crc8 value for a given buffer.
 * @param   crc0    initial crc value
 * @param   pbuf    pointer to the buffer
 * @param   len     buffer length
 * @return  crc value on success, -1 otherwise
 */
int computeCRC(const uint8_t crc0, const uint8_t *pbuf, int len);

#endif /* utils_crc8_h */