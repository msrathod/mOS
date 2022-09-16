/** 
 * @file 	mosconfig.h
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 10:55:47 am
 * -----
 * Last Modified: 16 09 2022, 05:16:21 pm
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
#define MOS_CONFIG_UART         (0)

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
 * Debug configuration 
 * @{
 */
/**
 * @def     MOS_DEBUG_VERBOSITY
 * @brief   Configures the Debug Verbosity
 * @param   state       0 - Debug functions ignored
 *                      1 - OPrint is enabled
 *                      2 - Dprint is also enabled
 *                      3 - Wprint is also enabled
 */ 
#define MOS_DEBUG_VERBOSITY     (0)
/** @} Debug configuration */


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