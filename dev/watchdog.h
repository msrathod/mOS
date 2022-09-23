/** 
 * @file 	watchdog.h
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 07:47:13 pm
 * -----
 * Last Modified: 23 09 2022, 07:57:01 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Watchdog APIs
 * 
 */
#ifndef dev_watchdog_h
#define dev_watchdog_h

/**
 * @fn      void watchdog_disable(void);
 * @brief   Disable the watchdog timer module.
 * @return  void
 */
void watchdog_disable(void);

/**
 * @fn      void watchdog_enable();
 * @brief   Enable the watchdog timer module.
 * @return  void
 */
void watchdog_enable(void);

/**
 * @brief   Pet the watchdog. Keeps it from resetting the system.
 * @fn      void watchdog_pet(void);
 * @return  void
 */
void watchdog_pet(void);

#endif /* dev_watchdog_h */