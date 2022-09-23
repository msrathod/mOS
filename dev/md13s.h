/** 
 * @file 	md13s.h
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 08:59:05 pm
 * -----
 * Last Modified: 23 09 2022, 09:21:10 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Driver for MD13s motor controller. Uses TimerA1
 *          for pwm signal generation on pin 9 (P2.1)
 * 
 */
#ifndef dev_md13s_h
#define dev_md13s_h
#include <stdint.h>

/**
 * @brief   MD13s command typedef
 */
typedef enum{
    MD_CW = 0x01,   /* clockwise */
    MD_CCW,         /* counter-clockwise */
    MD_STOP,        /* stop */
    MD_IDLE,
    MD_ERR
}mCmd_t;

/**
 * @fn      void md13s_init();
 * @brief   Initialize the md13s driver module.
 */
void md13s_init(void);

/**
 * @fn      void md13s_setDuty(uint8_t duty);
 * @brief   Set the duty cycle for pwm used in MD13s
 * @param   duty duty cycle to set
 * @return  void 
 */
void md13s_setDuty(uint8_t duty);

/**
 * @fn      void md13s_run(mCmd_t uCmd);
 * @brief   Control the MD13s dir and pwm.
 * @param   uCmd the command to run. @ref mCmd_t
 * @return  void
 */
void md13s_run(mCmd_t uCmd);

/**
 * @fn      void md13s_stop(void);
 * @brief   Stops the MD13s module.
 */
void md13s_stop(void);

#endif /* dev_md13s_h */