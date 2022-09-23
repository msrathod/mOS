/** 
 * @file 	mosconfig.h
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 10:55:47 am
 * -----
 * Last Modified: 23 09 2022, 10:14:16 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   System Configuration for mOS
 * 
 */
#ifndef mos_config_h
#define mos_config_h
#include <mosdefs.h>

/** 
 * CLOCK configuration 
 * @{
 */ 
/**
 * @def     MOS_MCLK_FREQ
 * @brief   Sets the Main Clock Frequency
 * @param   freq[MHz] || 16, 12, [8], 1 ||
 * @note    [x] => default frequency.
 */
#define MOS_MCLK_FREQ           (1)

/**
 * @def     MOS_ACLK_FREQ
 * @brief   Sets the Auxillary Clock Frequency
 * @param   freq[Hz] || [12000], 32768 ||
 * @note    [x] => default frequency.
 */
#define MOS_ACLK_FREQ           (12000)
/** @} */ //CLOCK Configuration

/** 
 * mOSS configuration 
 * @{
 */
/**
 * @def     MOS_APP_TASKS
 * @brief   Configures the max. mOS-Scheduler[mOSS] tasks required by
 *          the user application.          
 * @param   taskNum     {0, 1, [2], 3, 4}
 * @note    This doesn't include the tasks used by mOS itself.
 */ 
#define MOS_APP_TASKS           (2)

/** @} mOSS configuration */

/** 
 * UART configuration 
 * @{
 */
/**
 * @def     MOS_CONFIG_UART
 * @brief   Configures the state of UART module
 * @param   state       2 - UART Tx enabled
 *                      1 - UART Rx/Tx enabled
 *                      0 - UART disabled
 */ 
#define MOS_CONFIG_UART         (1)

#if MOS_USES(UART)
/**
 * @def     MOS_UART_BAUD
 * @brief   UART Baud Rate configuration 
 * @param   baud[bps] { [9600], 19200, 38400, 56000, 115200 }
 * @note    [x] => default baud.
 */
#define MOS_UART_BAUD           (115200)
#endif
/** @} */ //UART configuration

/** 
 * I2C configuration 
 * @{
 */
/**
 * @def     MOS_CONFIG_I2C
 * @brief   Configures the I2C module
 * @param   state       2 - I2C Slave mode enabled
 *                      1 - I2C Master mode enabled
 *                      0 - I2C module disabled
 */ 
#define MOS_CONFIG_I2C          (0)

#if MOS_USES(I2C)
/**
 * @def     MOS_I2C_SPEED
 * @brief   Configures the I2C bus speed for master mode.
 * @param   speed[Kbps] { [100000], 400000 }
 * @note    [x] => default speed.
 */
#define MOS_I2C_SPEED           (100000)
#endif
/** @} I2C configuration */

/** 
 * MD13S configuration 
 * @{
 */
/**
 * @def     MOS_CONFIG_MD13S
 * @brief   Configures the MD13S module
 * @param   state       1 - MD13S module enabled
 *                      0 - MD13S module disabled
 */ 
#define MOS_CONFIG_MD13S        (0)

#if MOS_USES(MD13S)
/**
 * @def     MOS_MD13S_FREQ 
 * @brief   MD13S PWM frequency configuration
 * @param   freq[Hz] { 250 to [2000] to 20000 }
 * @note    [x] => default frequency.
 */
#define MOS_MD13S_FREQ          (2000)
#endif /* MOS_USES(MD13S) */
/** @} MD13S configuration */

/** 
 * Printing configuration 
 * @{
 */
/**
 * @def     MOS_VERBOSITY
 * @brief   Configures the mOS Verbosity
 * @param   state       0 - All Print fns ignored
 *                      1 - MPrint is enabled
 *                      2 - OPrint is also enabled
 *                      3 - EPrint is also enabled
 */ 
#define MOS_VERBOSITY           (3)

#if MOS_USES(UART) == 0
#define MOS_VERBOSITY           (0)
#endif /* MOS_USES(UART) */

/**
 * @def     MOS_STDIO_CHNL
 * @brief   Set mOS standard io channel.
 * @param   state      [1] -> Serial (UART)
 *                      2  -> LCD (SPI) 
 */
#define MOS_STDIO_CHNL             (1)

/** @} Printing configuration */


/** 
 * Queue configuration 
 * @{
 */
/**
 * @def     MOS_MAX_QUEUE
 * @brief   Configures the max. number of queues supported. 
 * @param   qmax       { 0, 1, [2], 3, 4 }
 * @note    [x] => default queue nums.
 */ 
#define MOS_MAX_QUEUE           (3)
/** @} configuration */


/** 
 *  configuration 
 * @{
 */
/**
 * @def     
 * @brief   Configures the 
 * @param   state       0 - disabled
 *                      1 - enabled
 */ 

/** @} configuration */

#endif /* mos_config_h */