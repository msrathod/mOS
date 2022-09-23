/** 
 * @file 	i2c.h
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 10:20:51 pm
 * -----
 * Last Modified: 23 09 2022, 11:10:01 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   I2C driver module
 *          Provides both master and slave functionality.
 * 
 */
#ifndef dev_i2c_h
#define dev_i2c_h
#include <mosconfig.h>

#if MOS_USES(I2C) == 2
#include <i2cslave.h>
#elif MOS_USES(I2C) == 1
#include <i2cmaster.h>
#endif

#endif /* dev_i2c_h */
