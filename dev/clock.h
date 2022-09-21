/** 
 * @file 	clock.h
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 04:36:36 pm
 * -----
 * Last Modified: 21 09 2022, 09:08:35 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Clock driver for mOS
 */
#ifndef dev_clock_h
#define dev_clock_h
#include <mosconfig.h>

/* If invalid freq configured revert to default value  */
#if (MOS_GET(MCLK_FREQ)!=16) && \
    (MOS_GET(MCLK_FREQ)!=12) && \
    (MOS_GET(MCLK_FREQ)!=8) && \
    (MOS_GET(MCLK_FREQ)!=1)
#define MOS_MCLK_FREQ       (8)
#warning "Main clock configuration invalid reverting to default freq(8 MHz)."
#endif

#if (MOS_GET(ACLK_FREQ)!=32768) && \
    (MOS_GET(ACLK_FREQ)!=12000)
#define MOS_ACLK_FREQ        (12000)
#warning "Aux. clock configuration invalid reverting to default freq(12 KHz)."
#endif

/**
 * @fn      void clock_init(void);
 * @brief   Initialize clock module for mOS.
 * @return  void
 */
void clock_init(void);
/**
 * @fn      size_t getMCLK(void);
 * @brief   Get the MCLK value in MHz.
 * @return  Main Clock frequency (MHz)
 */
size_t getMCLK(void);
/**
 * @fn      size_t getACLK(void);
 * @brief   Get ACLK freq value in Hz.
 * @return  Auxillary clock frequency (Hz) 
 */
size_t getACLK(void);

#endif /* dev_clock_h */