/** 
 * @file 	sysled.h
 * @author 	Mohit Rathod
 * Created: 24 09 2022, 10:14:40 pm
 * -----
 * Last Modified: 24 09 2022, 10:19:46 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   SYS LED control APIs
 * 
 */
#ifndef dev_sys_led_h
#define dev_sys_led_h

/**
 * @fn      void sysled_ON(void);
 * @brief   Switch 'on' the system LED.
 */
void sysled_ON(void);

/**
 * @fn      void sysled_OFF(void);
 * @brief   Switch 'off' the system LED.
 */
void sysled_OFF(void);

/**
 * @fn      void sysled_TOGGLE(void);
 * @brief   Toggles the system LED.
 */
void sysled_TOGGLE(void);

#endif /* dev_sys_led_h */