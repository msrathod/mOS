/** 
 * @file 	tlv.h
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 07:38:39 pm
 * -----
 * Last Modified: 23 09 2022, 07:41:12 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Tag Length Value (TLV) integrity checking API
 * 
 */
#ifndef dev_tlv_h
#define dev_tlv_h

/**
 * @fn      int tlv_verify(void);
 * @brief   Verify the TLV data in flash storage.
 * @param   void
 * @return  0 on success, -1 otherwise 
 */
int tlv_verify(void);

#endif /* dev_tlv_h */